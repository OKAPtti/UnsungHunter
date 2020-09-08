#pragma once

namespace Alrescha {

	struct Matrix44;

	struct Vector3 {
		Vector3(void);
		Vector3(float aX, float aY, float aZ);
		Vector3(const D3DXVECTOR3& aD3DVector);
		Vector3(const Vector3& aVector3);
		~Vector3(void);

		Vector3& operator=(const Vector3& aAnother) { this->mD3DVector = aAnother.mD3DVector; return *this; }
		Vector3& operator=(const D3DXVECTOR3& aD3DVector) { this->mD3DVector = aD3DVector; return *this; }

		bool operator==(const Vector3& aAnother)const { return this->mD3DVector == aAnother.mD3DVector; }
		bool operator!=(const Vector3& aAnother)const { return this->mD3DVector != aAnother.mD3DVector; }

		Vector3 operator-(void)const { return { -this->mD3DVector }; }

		Vector3 operator+(const Vector3& aAnother)const { return { (this->mD3DVector + aAnother.mD3DVector) }; }
		Vector3& operator+=(const Vector3& aAnother) { this->mD3DVector += aAnother.mD3DVector; return *this; }

		Vector3 operator-(const Vector3& aAnother)const { return { (this->mD3DVector - aAnother.mD3DVector) }; }
		Vector3& operator-=(const Vector3& aAnother) { this->mD3DVector -= aAnother.mD3DVector; return *this; }

		Vector3 operator*(float aValue)const { return { (this->mD3DVector * aValue) }; }
		Vector3& operator*=(float aValue) { this->mD3DVector *= aValue; return *this; }
		Vector3 operator*(const Vector3& aAnother)const;
		Vector3& operator*=(const Vector3& aAnother);

		Vector3 operator/(float aValue)const { return { (this->mD3DVector / aValue) }; }
		Vector3& operator/=(float aValue) { this->mD3DVector /= aValue; return *this; }
		Vector3 operator/(const Vector3& aAnother)const;
		Vector3& operator/=(const Vector3& aAnother);

		//X,Y,Z全て0.0fか否か
		bool IsAllZero(void)const;
		//X,Y,Zのいずれかが0.0fか否か
		bool EvenOneZero(void)const;

		//引数の値で初期化する
		void ReInitialize(float aValur = 0.0f) { mD3DVector = { aValur,aValur,aValur }; }

		//正規化した時の値を返す
		Vector3 GetNormalize(void)const { return *D3DXVec3Normalize(&D3DXVECTOR3(), &mD3DVector); }
		//自分を正規化する
		void Normalize(void) { D3DXVec3Normalize(&mD3DVector, &mD3DVector); }

		float Dot(const Vector3& aVector3)const { return D3DXVec3Dot(&mD3DVector, &aVector3.mD3DVector); }

		//aIsFirstは引数を先に掛けるか否か
		Vector3 Cross(const Vector3& aVector3, bool aIsFirst)const;

		//行列で変換させた値を返す
		Vector3 GetTransform(const Matrix44& aMatrix44, bool aNormalize)const;
		//自分を行列で変換させる
		void Transform(const Matrix44& aMatrix44, bool aNormalize);

		//ルート計算をした結果を返す
		float Length(void)const { return D3DXVec3Length(&mD3DVector); }
		//ルート計算をしていない結果を返す
		float LengthSq(void)const { return D3DXVec3LengthSq(&mD3DVector); }

		void Clear(void) { x = 0.0f; y = 0.0f; z = 0.0f; }

		union {
			struct {
				float x, y, z;
			};
			D3DXVECTOR3 mD3DVector;
		};
	};

}