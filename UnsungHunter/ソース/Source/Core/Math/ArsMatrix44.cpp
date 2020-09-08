#include "ArsMatrix44.h"

namespace Alrescha
{

	Matrix44::Matrix44(void)
		:mD3DMatrix()
		, mScale({ 1.0f,1.0f,1.0f })
	{
		D3DXMatrixIdentity(&mD3DMatrix);
	}

	Matrix44::Matrix44(const Matrix44& aMatrix44)
		:mD3DMatrix(aMatrix44.mD3DMatrix)
		, mScale(aMatrix44.GetScale())
	{

	}

	Matrix44::Matrix44(const D3DXMATRIX& aD3DMatrix)
		: mD3DMatrix(aD3DMatrix)
		, mScale({ 1.0f,1.0f,1.0f })
	{

	}

	Matrix44::Matrix44(float a11, float a12, float a13, float a14,
		float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34,
		float a41, float a42, float a43, float a44)
		:mScale({ 1.0f,1.0f,1.0f })
	{
		m[0][0] = a11; m[0][1] = a12; m[0][2] = a13; m[0][3] = a14;
		m[1][0] = a21; m[1][1] = a22; m[1][2] = a23; m[1][3] = a24;
		m[2][0] = a31; m[2][1] = a32; m[2][2] = a33; m[2][3] = a34;
		m[3][0] = a41; m[3][1] = a42; m[3][2] = a43; m[3][3] = a44;
	}

	Matrix44::~Matrix44(void)
	{

	}

	Matrix44& Matrix44::operator=(const Matrix44& aAnother)
	{
		this->mD3DMatrix = aAnother.mD3DMatrix;
		this->mScale = aAnother.mScale;
		return *this;
	}

	Matrix44& Matrix44::operator=(const D3DXMATRIX& aD3DMatrix)
	{
		this->mD3DMatrix = aD3DMatrix;
		this->mScale = CalcScale(aD3DMatrix);
		return *this;
	}

	void Matrix44::SetPosition(const Vector3& aPosition)
	{
		_41 = aPosition.x;
		_42 = aPosition.y;
		_43 = aPosition.z;
	}

	void Matrix44::AddPosition(const Vector3& aAddValue, bool aTransToLocal)
	{
		//変更の必要が無かったら
		if (aAddValue.IsAllZero()) { return; }

		if (aTransToLocal) {
			D3DXMATRIX transMat;
			Translation(transMat, aAddValue);
			mD3DMatrix = transMat * mD3DMatrix;
		}
		else {
			_41 += aAddValue.x;
			_42 += aAddValue.y;
			_43 += aAddValue.z;
		}

	}

	Vector3 Matrix44::GetPosition(void)const
	{
		return Vector3{ mD3DMatrix._41,mD3DMatrix._42,mD3DMatrix._43 };
	}

	void Matrix44::SetScale(const Vector3& aScale)
	{
		//変更の必要が無かったら
		if (aScale == mScale ||
			aScale.EvenOneZero()) {
			return;
		}

		AddScale(aScale / mScale);
		mScale = aScale;
	}

	void Matrix44::AddScale(const Vector3& aAddValue)
	{
		//変更の必要が無かったら
		if (aAddValue.IsAllZero()) { return; }
		auto vec = aAddValue;
		if (vec.x == 0.0f) { vec.x = 1.0f; }
		if (vec.y == 0.0f) { vec.y = 1.0f; }
		if (vec.z == 0.0f) { vec.z = 1.0f; }

		D3DXMATRIX scaleMat;
		Scaling(scaleMat, vec);
		mD3DMatrix = scaleMat * mD3DMatrix;
		mScale += aAddValue;
	}

	void Matrix44::SetRotation(const Vector3 aRotationRadian)
	{
		D3DXMATRIX rotationMat;
		Rotation(rotationMat, aRotationRadian);
		SetRotation(rotationMat);
	}

	void Matrix44::SetRotation(const Matrix44& aMatrix44)
	{
		const Vector3 CurrentPosition = GetPosition();
		const Vector3 CurrentScale = mScale;

		mD3DMatrix = aMatrix44.mD3DMatrix;
		mScale.ReInitialize(1.0f);

		SetPosition(CurrentPosition);
		SetScale(CurrentScale);
	}

	void Matrix44::AddRotation(const Vector3& aAddRadian, bool aTransToLocal)
	{
		//変更の必要が無かったら
		if (aAddRadian.IsAllZero()) { return; }

		D3DXMATRIX rotateMat;
		Rotation(rotateMat, aAddRadian);

		if (aTransToLocal) { mD3DMatrix = rotateMat * mD3DMatrix; }
		else { mD3DMatrix = mD3DMatrix * rotateMat; }
	}

	void Matrix44::RotationByPosition(const Matrix44& aTransform)
	{
		RotationByPosition(aTransform.GetPosition());
	}

	void Matrix44::RotationByPosition(const Vector3& aPosition)
	{
		const auto CurrentPosition = GetPosition();
		if (aPosition == CurrentPosition) { return; }

		//ポジション方向を向いた時の、ワールドから見た各軸のベクトル
		Vector3 localX, localY, localZ;
		//Z軸の方向(自分から見た相手の向き)
		(localZ = (aPosition - CurrentPosition)).Normalize();
		//X軸の方向(Z軸とワールドY軸に垂直な軸)
		(localX = localZ.Cross({ 0.0f,1.0f,0.0f }, true)).Normalize();
		//Y軸の方向(X軸とZ軸に垂直な軸)
		(localY = localX.Cross(localZ, true)).Normalize();

		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		m._11 = localX.x; m._12 = localX.y; m._13 = localX.z;
		m._21 = localY.x; m._22 = localY.y; m._23 = localY.z;
		m._31 = localZ.x; m._32 = localZ.y; m._33 = localZ.z;

		SetRotation(m);
	}

	void Matrix44::RotationByVector(const Vector3& aVector, bool aIsLocalVector)
	{
		if (aVector.IsAllZero()) { return; }
		if (aIsLocalVector) { RotationByPosition(aVector.GetTransform(*this, false)); }
		else { RotationByPosition(GetPosition() + aVector); }
	}

	void Matrix44::RotationYAxisByPosition(const Matrix44& aTransform, float aMaxRotateRadian)
	{
		RotationYAxisByPosition(aTransform.GetPosition(), aMaxRotateRadian);
	}

	void Matrix44::RotationYAxisByPosition(const Vector3& aPosition, float aMaxRotateRadian)
	{
		//前方のベクトル
		auto tmpVec = Transform({ 0.0f,0.0f,1.0f }, true);
		tmpVec.y = 0.0f;
		const auto ForwardVector = tmpVec.GetNormalize();
		//目的方向のベクトル
		tmpVec = (aPosition - GetPosition());
		tmpVec.y = 0.0f;
		const auto TargetVector = tmpVec.GetNormalize();

		const float Dot = ForwardVector.Dot(TargetVector);

		//誤差の場合は回転させない
		constexpr float Epsilon = 0.001f;
		if (Dot > (1.0f - Epsilon)) { return; }

		float radian = acosf(Dot);
		//たまにnanを返すのでチェック(Dotの値が-1.0f～1.0fの範囲外になる？)
		if (isnan(radian)) { return; }

		//振り向き速度の制限
		if (radian > aMaxRotateRadian) { radian = aMaxRotateRadian; }

		//どちらの方向に回転させるか
		const auto Cross = ForwardVector.Cross(TargetVector, true);
		if (Cross.y > 0.0f) { radian *= -1.0f; }

		Matrix44 rotate;
		D3DXMatrixRotationY(&rotate.mD3DMatrix, radian);
		*this = rotate * *this;
	}

	void Matrix44::RotationYAxisByVector(const Vector3& aVector, bool aIsLocalVector, float aMaxRotateRadian)
	{
		if (aVector.IsAllZero()) { return; }
		if (aIsLocalVector) { RotationYAxisByPosition(aVector.GetTransform(*this, false), aMaxRotateRadian); }
		else { RotationYAxisByPosition(GetPosition() + aVector, aMaxRotateRadian); }
	}

	Vector3 Matrix44::Transform(const Vector3& aVector, bool aNormalize)const
	{
		return aVector.GetTransform(*this, aNormalize);
	}

	Matrix44 Matrix44::GetInverse(void)const
	{
		return Matrix44(*D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &mD3DMatrix));
	}

	Vector3 Matrix44::GetCenter(void)const
	{
		return Transform({ 0.0f,0.0f,1.0f }, true);
	}

	void Matrix44::Translation(D3DXMATRIX& aD3DMatrix, const Vector3& aPosition)const
	{
		D3DXMatrixIdentity(&aD3DMatrix);
		aD3DMatrix._41 = aPosition.x;
		aD3DMatrix._42 = aPosition.y;
		aD3DMatrix._43 = aPosition.z;
	}

	void Matrix44::Scaling(D3DXMATRIX& aD3DMatrix, const Vector3& aScale)const
	{
		D3DXMatrixIdentity(&aD3DMatrix);
		aD3DMatrix._11 = aScale.x;
		aD3DMatrix._22 = aScale.y;
		aD3DMatrix._33 = aScale.z;
	}

	void Matrix44::Rotation(D3DXMATRIX& aD3DMatrix, const Vector3& aRotationRadian)const
	{
		D3DXMatrixIdentity(&aD3DMatrix);

		D3DXMATRIX tmpMat;
		D3DXMatrixRotationX(&tmpMat, aRotationRadian.x);
		aD3DMatrix *= tmpMat;
		D3DXMatrixRotationY(&tmpMat, aRotationRadian.y);
		aD3DMatrix *= tmpMat;
		D3DXMatrixRotationZ(&tmpMat, aRotationRadian.z);
		aD3DMatrix *= tmpMat;
	}

	Vector3 Matrix44::CalcScale(const D3DXMATRIX& aD3DMatrix)
	{
		return {
			sqrtf((aD3DMatrix._11 * aD3DMatrix._11) + (aD3DMatrix._12 * aD3DMatrix._12) + (aD3DMatrix._13 * aD3DMatrix._13)),
			sqrtf((aD3DMatrix._21 * aD3DMatrix._21) + (aD3DMatrix._22 * aD3DMatrix._22) + (aD3DMatrix._23 * aD3DMatrix._23)),
			sqrtf((aD3DMatrix._31 * aD3DMatrix._31) + (aD3DMatrix._32 * aD3DMatrix._32) + (aD3DMatrix._33 * aD3DMatrix._33))
		};
	}
}
