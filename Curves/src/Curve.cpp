#include "hzpch.h"
#include "Curve.h"


Curve::~Curve()
{
}

Circle::Circle(float radius)
	: m_Radius(radius)
{
	if (radius < EPSILON)
		throw IllegalCurveParameterException();
}

Circle::Circle(Circle&& rhs) noexcept
	: m_Radius(rhs.m_Radius)
{

}

Circle::Circle(const Circle& rhs)
	: m_Radius(rhs.m_Radius)
{
}

Circle& Circle::operator=(const Circle& rhs)
{
	m_Radius = rhs.m_Radius;
	return *this;
}

Circle& Circle::operator=(Circle&& rhs) noexcept
{
	m_Radius = rhs.m_Radius;
	return  *this;
}

Vector3d Circle::GetPoint(float t) const
{
	float x = m_Radius * sin(t);
	float y = m_Radius * cos(t);
	return Vector3d{ x, y, 0.f };
}

Vector3d Circle::GetGradient(float t) const
{
	float x = m_Radius * -sin(t);
	float y = m_Radius * cos(t);
	return { x, y , 0};
}

bool operator<(const Circle& left, const Circle& right)
{
	return left.m_Radius < right.m_Radius;
}

/*--------------------------Ellipse--------------------------------*/

Ellipse::Ellipse(float horizontal_radius, float vertical_radius)
	: m_RadiusHorizontal(horizontal_radius), m_RadiusVertical(vertical_radius)
{
	if (m_RadiusHorizontal < EPSILON || m_RadiusVertical < EPSILON)
		throw IllegalCurveParameterException();
}

Vector3d Ellipse::GetPoint(float t) const
{
	float x = m_RadiusHorizontal * cos(t);
	float y = m_RadiusVertical * sin(t);
	return { x, y, 0.f };
}

Vector3d Ellipse::GetGradient(float t) const
{
	float x = m_RadiusHorizontal * -sin(t);
	float y = m_RadiusVertical * cos(t);
	return { x, y, 0.f };
}

/*--------------------------Helix--------------------------------*/

Helix::Helix(float radius, float step)
	: m_Radius(radius), m_Step(step), m_HeightSlope(step / (2.f * M_PI))
{
	if (m_Radius < EPSILON || m_Step < EPSILON)
		throw IllegalCurveParameterException();
}

Vector3d Helix::GetPoint(float t) const
{
	float x = m_Radius * cos(t);
	float y = m_Radius * sin(t);
	return { x, y, m_HeightSlope * t };
}

Vector3d Helix::GetGradient(float t) const
{
	float x = m_Radius * -sin(t);
	float y = m_Radius * cos(t);
	return { x, y, m_HeightSlope };
}

std::ostream& operator<<(std::ostream& out, const Vector3d& vec3d)
{
	out << vec3d.x << ", ";
	out << vec3d.y << ", ";
	out << vec3d.z;
	return out;
}

