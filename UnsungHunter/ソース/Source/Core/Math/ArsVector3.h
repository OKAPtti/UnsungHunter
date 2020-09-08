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

		//X,Y,Z�S��0.0f���ۂ�
		bool IsAllZero(void)const;
		//X,Y,Z�̂����ꂩ��0.0f���ۂ�
		bool EvenOneZero(void)const;

		//�����̒l�ŏ���������
		void ReInitialize(float aValur = 0.0f) { mD3DVector = { aValur,aValur,aValur }; }

		//���K���������̒l��Ԃ�
		Vector3 GetNormalize(void)const { return *D3DXVec3Normalize(&D3DXVECTOR3(), &mD3DVector); }
		//�����𐳋K������
		void Normalize(void) { D3DXVec3Normalize(&mD3DVector, &mD3DVector); }

		float Dot(const Vector3& aVector3)const { return D3DXVec3Dot(&mD3DVector, &aVector3.mD3DVector); }

		//aIsFirst�͈������Ɋ|���邩�ۂ�
		Vector3 Cross(const Vector3& aVector3, bool aIsFirst)const;

		//�s��ŕϊ��������l��Ԃ�
		Vector3 GetTransform(const Matrix44& aMatrix44, bool aNormalize)const;
		//�������s��ŕϊ�������
		void Transform(const Matrix44& aMatrix44, bool aNormalize);

		//���[�g�v�Z���������ʂ�Ԃ�
		float Length(void)const { return D3DXVec3Length(&mD3DVector); }
		//���[�g�v�Z�����Ă��Ȃ����ʂ�Ԃ�
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