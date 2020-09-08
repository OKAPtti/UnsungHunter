#pragma once

namespace Alrescha {
	struct Matrix44
	{
		Matrix44(void);
		Matrix44(const Matrix44& aMatrix44);
		Matrix44(const D3DXMATRIX& aD3DMatrix);
		Matrix44(float a11, float a12, float a13, float a14,
			float a21, float a22, float a23, float a24,
			float a31, float a32, float a33, float a34,
			float a41, float a42, float a43, float a44);
		~Matrix44(void);

		Matrix44& operator=(const Matrix44& aAnother);
		Matrix44& operator=(const D3DXMATRIX& aD3DMatrix);
		Matrix44 operator*(const Matrix44& aAnother)const { return { (this->mD3DMatrix * aAnother.mD3DMatrix) }; }
		Matrix44 operator*(const D3DXMATRIX& aD3DMatrix)const { return { (this->mD3DMatrix * aD3DMatrix) }; }

		void SetPosition(const Vector3& aPosition);
		//aTransToLocal��false�Ȃ烏�[���h�ϊ�
		void AddPosition(const Vector3& aAddValue, bool aTransToLocal = true);
		Vector3 GetPosition(void)const;

		void SetScale(const Vector3& aScale);
		void AddScale(const Vector3& aAddValue);
		Vector3 GetScale(void)const { return mScale; }

		void SetRotation(const Vector3 aRotationRadian);
		void SetRotation(const Matrix44& aMatrix44);
		//aTransToLocal��false�Ȃ烏�[���h�ϊ�
		void AddRotation(const Vector3& aAddRadian, bool aTransToLocal = true);

		//�s����W�̕�������������
		void RotationByPosition(const Matrix44& aTransform);
		//���W�̕�������������
		void RotationByPosition(const Vector3& aPosition);
		//�x�N�g���̕�������������
		void RotationByVector(const Vector3& aVector, bool aIsLocalVector);

		//Y���̂ݍs����W�̕�������������B��������1�x�ɉ�]�ł���ő�x��
		void RotationYAxisByPosition(const Matrix44& aTransform, float aMaxRotateRadian = FLT_MAX);
		//Y���̂ݍ��W�̕�������������B��������1�x�ɉ�]�ł���ő�x��
		void RotationYAxisByPosition(const Vector3& aPosition, float aMaxRotateRadian = FLT_MAX);
		//Y���̂݃x�N�g���̕�������������B��������1�x�ɉ�]�ł���ő�x��
		void RotationYAxisByVector(const Vector3& aVector, bool aIsLocalVector, float aMaxRotateRadian = FLT_MAX);

		void Identity(void) { D3DXMatrixIdentity(&mD3DMatrix); }

		//Vector3���s��ŕϊ�������
		Vector3 Transform(const Vector3& aVector, bool aNormalize)const;

		//�t�s��̍쐬
		Matrix44 GetInverse(void)const;
		//���g���t�s��ɂ���
		void Inverse(void) { D3DXMatrixInverse(&mD3DMatrix, nullptr, &mD3DMatrix); }

		Vector3 GetCenter(void)const;

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;

			};
			float m[4][4];
			D3DXMATRIX mD3DMatrix;
		};

	private:
		//�ړ��s��̍쐬
		void Translation(D3DXMATRIX& aD3DMatrix, const Vector3& aPosition)const;
		//�g�k�s��̍쐬
		void Scaling(D3DXMATRIX& aD3DMatrix, const Vector3& aScale)const;
		//��]�s��̍쐬
		void Rotation(D3DXMATRIX& aD3DMatrix, const Vector3& aRotationRadian)const;

		//�X�P�[���l���v�Z����
		Vector3 CalcScale(const D3DXMATRIX& aD3DMatrix);

		//�X�P�[���𓱂��R�X�g�������̂Ŏ����Ă���(����ɏ��������Ăق����Ȃ��̂�private)
		Vector3 mScale;
	};

}