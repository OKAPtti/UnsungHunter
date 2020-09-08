/**
* @file ArsCameraSystem.h
* @brief カメラを制御するクラス
*/

#pragma once

namespace Alrescha {
	class GameObject;
	class CameraSystem
	{
	public:
		CameraSystem(void);
	   ~CameraSystem(void);

		
		/**
		* @brief 画面回転の更新
		* @param マウスの移動量
		*/
		void UpdateRotation(const D3DXVECTOR2& aAddRadian);

		
		/**
		* @brief ビュー行列の更新
		*/
		void UpdateViewMatrix(void);

		/**
		* @brief 注視点の指定
		* @param 注視させたいオブジェクト
		*/
		void SetTargetViewpoint(const WPtr<const GameObject>& apTargetViewpoint) { mpTargetViewpoint = apTargetViewpoint; }

		/**
		* @brief ビュー行列を返します
		*/
		const D3DXMATRIX& GetViewMatrix(void)const;
		
		/**
		* @brief 注視点の座標を返します
		*/
		const D3DXVECTOR3& GetViewpoint(void);

		/**
		* @brief カメラの回転行列を返します
		*/
		const D3DXMATRIX& GetCamRot(void);

		/**
		* @brief カメラから見たベクトルを返します
		*/
		const D3DXVECTOR3& GetCenterVector(void)const;

		/**
		* @brief 視野角を返します
		*/
		float GetViewingAngle(void)const;

		/**
		* @brief 視野角を返します
		* @param 移動させるベクトル(場所)
		* @param 終了時間
		*/
		void SetTransPosition(const D3DXVECTOR3& aTransPosition, int aEndCount);

		/**
		* @brief 画面中央を拡大します
		* @note 視野角を変更し、拡大させている
		*/
		void ZoomIn(void);
		
		/**
		* @brief 画面中央を縮小します
		* @note 視野角を変更し、縮小させている
		*/
		void ZoomOut(void);

		/**
		* @brief 通常の状態でのマウス感度
		*/
		void NormalSensitivity(void)	{ mAdjustmentValue = 0.001f; }

		/**
		* @brief 弓を構えている時のマウス感度
		*/
		void BendSensitivity(void)		{ mAdjustmentValue = 0.0005f; }

	private:
		//注視点の対象.
		WPtr<const GameObject>	mpTargetViewpoint;
		//注視点の座標.
		D3DXVECTOR3				mViewpoint;

		//ビュー行列.
		D3DXMATRIX				mViewMatrix;
		//カメラの回転行列.
		D3DXMATRIX				mCamRot;

		//カメラの角度.
		D3DXVECTOR2				mCamAng;

		//カメラの位置を一時的に滑らかに変更.
		D3DXVECTOR3				mTransPosition;
		Lerper<D3DXVECTOR3>		mTransPositionLerper;

		//マウス感度.
		float					mSensitivity;
		//マウス感度の調整値.
		float					mAdjustmentValue;

		//視野角.
		float					mViewingAngle;
		Lerper<float>			mAngleLerper;
	};

}