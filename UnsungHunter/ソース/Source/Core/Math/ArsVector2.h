#pragma once

namespace Alrescha
{
	struct Matrix44;

	struct Vector2
	{
		Vector2(void);
		Vector2(float aX, float aY);
		Vector2(const D3DXVECTOR2& aD3DVector);
		Vector2(const Vector2& aVector3);
		~Vector2(void);

		Vector2& operator=(const Vector2& aAnother) { this->mD3DVector = aAnother.mD3DVector; return *this; }
		Vector2& operator=(const D3DXVECTOR2& aD3DVector) { this->mD3DVector = aD3DVector; return *this; }

		bool operator==(const Vector2& aAnother)const { return this->mD3DVector == aAnother.mD3DVector; }
		bool operator!=(const Vector2& aAnother)const { return this->mD3DVector != aAnother.mD3DVector; }

		Vector2 operator-(void)const { return { -this->mD3DVector }; }

		Vector2 operator+(const Vector2& aAnother)const { return { (this->mD3DVector + aAnother.mD3DVector) }; }
		Vector2& operator+=(const Vector2& aAnother) { this->mD3DVector += aAnother.mD3DVector; return *this; }

		Vector2 operator-(const Vector2& aAnother)const { return { (this->mD3DVector - aAnother.mD3DVector) }; }
		Vector2& operator-=(const Vector2& aAnother) { this->mD3DVector -= aAnother.mD3DVector; return *this; }

		Vector2 operator*(float aValue)const { return { (this->mD3DVector * aValue) }; }
		Vector2& operator*=(float aValue) { this->mD3DVector *= aValue; return *this; }
		Vector2 operator*(const Vector2& aAnother)const;
		Vector2& operator*=(const Vector2& aAnother);

		Vector2 operator/(float aValue)const { return { (this->mD3DVector / aValue) }; }
		Vector2& operator/=(float aValue) { this->mD3DVector /= aValue; return *this; }
		Vector2 operator/(const Vector2& aAnother)const;
		Vector2& operator/=(const Vector2& aAnother);

		//X,Y全て0.0fか否か
		bool IsAllZero(void)const;

		//引数の値で初期化する
		void ReInitialize(float aValur = 0.0f) { mD3DVector = { aValur,aValur }; }

		//正規化した時の値を返す
		Vector2 GetNormalize(void)const { return *D3DXVec2Normalize(&D3DXVECTOR2(), &mD3DVector); }
		//自分を正規化する
		void Normalize(void) { D3DXVec2Normalize(&mD3DVector, &mD3DVector); }

		float Dot(const Vector2& aVector2) { return D3DXVec2Dot(&mD3DVector, &aVector2.mD3DVector); }

		float Cross(const Vector2& aVector2);

		//ルート計算をした結果を返す
		float Length(void) { return D3DXVec2Length(&mD3DVector); }
		//ルート計算をしていない結果を返す
		float LengthSq(void) { return D3DXVec2LengthSq(&mD3DVector); }

		union {
			struct {
				float x, y;
			};
			D3DXVECTOR2 mD3DVector;
		};
	};
}