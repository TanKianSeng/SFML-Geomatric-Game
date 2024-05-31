#include "Vec2.h"
#include <math.h>
#include <cmath>

Vec2::Vec2()
{
		
}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin)
{

}

Vec2 Vec2::operator + (const Vec2 & rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator / (const float val) const
{
	return Vec2(x / val, y / val);
}

bool Vec2::operator ==(const Vec2& rhs)const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator !=(const Vec2& rhs)const
{
	return (x != rhs.x && y != rhs.y);
}

void Vec2::operator += (const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}
void Vec2::operator -= (const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;

}
void Vec2::operator *= (const Vec2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
}
void Vec2::operator /= (const Vec2& rhs) 
{
	x /= rhs.x;
	y /= rhs.y;
}

float Vec2::dist() const
{
	return sqrtf(x*x + y*y);
}

float Vec2::distNoSqrt() const
{
	return (x*x + y*y);
}





Vec2 Vec2::angleVelocity(const float d) const
{
	
	const double pi = 3.14159265358979323846;
	// Convert launch angle from degrees to radians
	float launchAngleRadians = d * pi / 180.0;

	// Calculate horizontal and vertical components of velocity
	float velocityX =  cos(launchAngleRadians);
	float velocityY =  sin(launchAngleRadians);

	
	
	return Vec2(velocityX,velocityY);
}
