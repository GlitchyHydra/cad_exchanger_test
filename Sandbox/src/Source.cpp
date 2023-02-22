#include "hzpch.h"

#include <iostream>

#include "Curve.h"

#include "oneapi/tbb/parallel_reduce.h"
#include "oneapi/tbb/blocked_range.h"

std::random_device rd;

std::mt19937_64 eng = std::mt19937_64(rd());

enum curve_type
{
	CIRCLE = 0,
	ELLIPSE = 1,
	HELIX = 2,
	COUNT = 3
};

using curve_ref = std::shared_ptr<Curve>;

void generate_curves_randomly(std::vector<curve_ref>& curves, size_t size = 10)
{
	std::uniform_int_distribution<int> distr(0, INT_MAX);
	std::uniform_real_distribution<float> float_distr(1.f, 40.f);
	for (size_t i = 0; i < size; ++i)
	{
		auto curveType = distr(eng) % curve_type::COUNT;
		switch (curveType)
		{
			case curve_type::CIRCLE:
				{
					float radius = float_distr(eng);
					curves.emplace_back(std::make_shared<Circle>(radius));
				} break;
			case curve_type::ELLIPSE:
				{
					float radiusHorizontal = float_distr(eng);
					float radiusVertical = float_distr(eng);
					curves.emplace_back(std::make_shared<Ellipse>(radiusHorizontal, radiusVertical));
				} break;
			case curve_type::HELIX:
				{
					float radius = float_distr(eng);
					float step = float_distr(eng);
					curves.emplace_back(std::make_shared<Helix>(radius, step));
				} break;
		}
	}
	
}

void print_all_at_t(std::vector<curve_ref>& curves, float t = M_PI / 4.f)
{
	for (auto& curve : curves)
	{
		std::cout << "Point: (" << curve->GetPoint(t) << ")" << std::endl;
		std::cout << "Gradient: (" << curve->GetGradient(t) << ")" << std::endl;
	}
}

void pick_circles(std::vector<curve_ref>& curves, std::vector<std::shared_ptr<Circle>>& circles)
{
	for (auto& curve : curves)
	{
		auto circle = std::dynamic_pointer_cast<Circle>(curve);
		if (!circle)
			continue;
		circles.emplace_back(circle);
	}
}

bool circle_comp(const std::shared_ptr<Circle>& lhs, const std::shared_ptr<Circle>& rhs)
{
	return *lhs < *rhs;
}


float calculate_total_sum(const std::vector<std::shared_ptr<Circle>>& circles)
{
	float totalSum = 0.f;
	for (auto& circle : circles)
	{
		totalSum += circle->GetRadius();
	}
	return totalSum;
}

struct Sum
{
	float m_TotalSum;

	Sum() : m_TotalSum(0) {}
	Sum(Sum& s, tbb::split) { m_TotalSum = 0; }

	void operator()(const tbb::blocked_range<std::vector<std::shared_ptr<Circle>>::const_iterator>& r)
	{
		float tempSum = m_TotalSum;
		for (auto circle = r.begin(); circle != r.end(); ++circle)
		{
			tempSum += circle->get()->GetRadius();
		}
		m_TotalSum = tempSum;
	}

	void join(Sum& rhs) { m_TotalSum += rhs.m_TotalSum; }
};

float calculate_total_sum_concurrently(const std::vector<std::shared_ptr<Circle>>& circles)
{
	Sum sum;
	tbb::parallel_reduce(tbb::blocked_range(circles.begin(), circles.end()), sum);
	return sum.m_TotalSum;
}

int main()
{
	//2. Populate a container (e.g. vector or list) of objects of these types created in random manner with
	//random parameters.
	std::vector<curve_ref> curves;
	generate_curves_randomly(curves, 100);

	//3. Print coordinates of points and derivatives of all curves in the container at t=PI/4.
	print_all_at_t(curves);

	//4. Populate a second container that would contain only circles from the first container. Make sure the
	//second container shares(i.e. not clones) circles of the first one, e.g.via pointers
	std::vector<std::shared_ptr<Circle>> circles;
	pick_circles(curves, circles);

	//5. Sort the second container in the ascending order of circles’ radii. That is, the first element has the
	//smallest radius, the last - the greatest.
	std::sort(circles.begin(), circles.end(), circle_comp);

	//6. Compute the total sum of radii of all curves in the second container.
	float sum1 = calculate_total_sum(circles);
	std::cout << "Total sum (one thread): " << sum1 << std::endl;

	//8. Implement computation of the total sum of radii using parallel computations (Intel TBB library).
	float sum2 = calculate_total_sum_concurrently(circles);
	std::cout << "Total sum (multithreaded): " << sum2 << std::endl;
}