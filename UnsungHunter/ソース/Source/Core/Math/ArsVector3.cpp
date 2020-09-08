#include"ArsVector3.h"

namespace Alrescha {

	Vector3::Vector3(void)
		:mD3DVector({ 0.0f,0.0f,0.0f })
	{

	}

	Vector3::Vector3(float aX, float aY, float aZ)
		: mD3DVector({ aX,aY,aZ })
	{

	}

	Vector3::Vector3(const D3DXVECTOR3& aD3DVector)
		: mD3DVector(aD3DVector)
	{

	}

	Vector3::Vector3(const Vector3& aVector3)
		: mD3DVector(aVector3.mD3DVector)
	{

	}

	Vector3::~Vector3(void)
	{

	}

	Vector3 Vector3::operator*(const Vector3& aAnother) const
	{
		return {
			this->x * aAnother.x,
			this->y * aAnother.y,
			this->z * aAnother.z
		};
	}

	Vector3& Vector3::operator*=(const Vector3& aAnother)
	{
		*this = *this * aAnother;
		return *this;
	}

	Vector3 Vector3::operator/(const Vector3& aAnother) const
	{
		return {
			this->x / aAnother.x,
			this->y / aAnother.y,
			this->z / aAnother.z
		};
	}

	Vector3& Vector3::operator/=(const Vector3& aAnother)
	{
		*this = *this / aAnother;
		return *this;
	}

	bool Vector3::IsAllZero(void) const
	{
		return (
			x == 0.0f &&
			y == 0.0f &&
			z == 0.0f
			);
	}

	bool Vector3::EvenOneZero(void) const
	{
		return (x == 0.0f ||
			y == 0.0f ||
			z == 0.0f);
	}

	Vector3 Vector3::Cross(const Vector3& aVector3, bool aIsFirst) const
	{
		D3DXVECTOR3 crossVec;
		if (aIsFirst) {
			D3DXVec3Cross(&crossVec, &aVector3.mD3DVector, &mD3DVector);
		}
		else {
			D3DXVec3Cross(&crossVec, &mD3DVector, &aVector3.mD3DVector);
		}
		return { crossVec };
	}

	Vector3 Vector3::GetTransform(const Matrix44& aMatrix44, bool aNormalize)const
	{
		Vector3 transVec = *this;
		transVec.Transform(aMatrix44, aNormalize);
		return transVec;
	}

	void Vector3::Transform(const Matrix44& aMatrix44, bool aNormalize)
	{
		if (aNormalize) {
			D3DXVec3TransformNormal(&mD3DVector, &mD3DVector, &aMatrix44.mD3DMatrix);
		}
		else {
			D3DXVec3TransformCoord(&mD3DVector, &mD3DVector, &aMatrix44.mD3DMatrix);
		}
	}

}