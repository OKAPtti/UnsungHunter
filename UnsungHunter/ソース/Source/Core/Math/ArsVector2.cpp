#include "ArsVector2.h"

namespace Alrescha
{
	Vector2::Vector2(void)
		:mD3DVector({ 0.0f,0.0f })
	{

	}

	Vector2::Vector2(float aX, float aY)
		: mD3DVector({ aX,aY })
	{

	}

	Vector2::Vector2(const D3DXVECTOR2& aD3DVector)
		: mD3DVector(aD3DVector)
	{

	}

	Vector2::Vector2(const Vector2& aVector3)
		: mD3DVector(aVector3.mD3DVector)
	{

	}

	Vector2::~Vector2(void)
	{

	}

	Vector2 Vector2::operator*(const Vector2& aAnother) const
	{
		return {
			this->x * aAnother.x,
			this->y * aAnother.y
		};
	}

	Vector2& Vector2::operator*=(const Vector2& aAnother)
	{
		*this = *this * aAnother;
		return *this;
	}

	Vector2 Vector2::operator/(const Vector2& aAnother) const
	{
		return {
			this->x / aAnother.x,
			this->y / aAnother.y
		};
	}

	Vector2& Vector2::operator/=(const Vector2& aAnother)
	{
		*this = *this / aAnother;
		return *this;
	}

	bool Vector2::IsAllZero(void) const
	{
		return (
			x == 0.0f &&
			y == 0.0f
			);
	}

	float Vector2::Cross(const Vector2& aVector2)
	{
		return {
			(x * aVector2.y) -
			(y * aVector2.x)
		};
	}
}