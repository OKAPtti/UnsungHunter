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
		//aTransToLocalがfalseならワールド変換
		void AddPosition(const Vector3& aAddValue, bool aTransToLocal = true);
		Vector3 GetPosition(void)const;

		void SetScale(const Vector3& aScale);
		void AddScale(const Vector3& aAddValue);
		Vector3 GetScale(void)const { return mScale; }

		void SetRotation(const Vector3 aRotationRadian);
		void SetRotation(const Matrix44& aMatrix44);
		//aTransToLocalがfalseならワールド変換
		void AddRotation(const Vector3& aAddRadian, bool aTransToLocal = true);

		//行列座標の方向を向かせる
		void RotationByPosition(const Matrix44& aTransform);
		//座標の方向を向かせる
		void RotationByPosition(const Vector3& aPosition);
		//ベクトルの方向を向かせる
		void RotationByVector(const Vector3& aVector, bool aIsLocalVector);

		//Y軸のみ行列座標の方向を向かせる。第二引数は1度に回転できる最大度合
		void RotationYAxisByPosition(const Matrix44& aTransform, float aMaxRotateRadian = FLT_MAX);
		//Y軸のみ座標の方向を向かせる。第二引数は1度に回転できる最大度合
		void RotationYAxisByPosition(const Vector3& aPosition, float aMaxRotateRadian = FLT_MAX);
		//Y軸のみベクトルの方向を向かせる。第二引数は1度に回転できる最大度合
		void RotationYAxisByVector(const Vector3& aVector, bool aIsLocalVector, float aMaxRotateRadian = FLT_MAX);

		void Identity(void) { D3DXMatrixIdentity(&mD3DMatrix); }

		//Vector3を行列で変換させる
		Vector3 Transform(const Vector3& aVector, bool aNormalize)const;

		//逆行列の作成
		Matrix44 GetInverse(void)const;
		//自身を逆行列にする
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
		//移動行列の作成
		void Translation(D3DXMATRIX& aD3DMatrix, const Vector3& aPosition)const;
		//拡縮行列の作成
		void Scaling(D3DXMATRIX& aD3DMatrix, const Vector3& aScale)const;
		//回転行列の作成
		void Rotation(D3DXMATRIX& aD3DMatrix, const Vector3& aRotationRadian)const;

		//スケール値を計算する
		Vector3 CalcScale(const D3DXMATRIX& aD3DMatrix);

		//スケールを導くコストが高いので持っておく(下手に書き換えてほしくないのでprivate)
		Vector3 mScale;
	};

}