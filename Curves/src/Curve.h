#pragma once

#ifdef CURVELIBRARY_EXPORTS
#define CURVELIBRARY_API __declspec(dllexport)
#else 
#define CURVELIBRARY_API __declspec(dllimport)
#endif

#define EPSILON 0.00001f
#define M_PI 3.14159265358979323846f 

class IllegalCurveParameterException : public std::exception
{
public:
	char* what()
	{
		return "One of curve parameters is illegal";
	}
};

struct Vector3d
{
	float x, y, z;
};

class CURVELIBRARY_API Curve
{
public:
	virtual ~Curve();

	virtual Vector3d GetPoint(float t) const = 0;
	virtual Vector3d GetGradient(float t) const = 0;
};

class CURVELIBRARY_API Circle : public Curve
{
	float m_Radius;
public:
	Circle(float radius);

	friend bool CURVELIBRARY_API operator<(const Circle& lhs, const Circle& rhs);

	virtual Vector3d GetPoint(float t) const override;
	virtual Vector3d GetGradient(float t) const override;

	float GetRadius() { return m_Radius; }
};

class CURVELIBRARY_API Ellipse : public Curve
{
	float m_RadiusHorizontal;
	float m_RadiusVertical;
public:
	Ellipse(float horizontalRadius, float verticalRadius);

	virtual Vector3d GetPoint(float t) const override;
	virtual Vector3d GetGradient(float t) const override;
};

class CURVELIBRARY_API Helix : public Curve
{
	float m_Radius;
	float m_Step;
	float m_HeightSlope;

public:
	Helix(float radius, float step);
		
	virtual Vector3d GetPoint(float t) const override;
	virtual Vector3d GetGradient(float t) const override;
};

CURVELIBRARY_API std::ostream& operator<< (std::ostream& out, const Vector3d& vec3d);