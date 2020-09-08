#pragma once
#include "../ArsWeapon.h"

namespace Alrescha {

	class BaseArrow :public Weapon
	{
	public:
		BaseArrow(int aType);
		virtual ~BaseArrow(void)override;

		/**
		* @brief 初期化をします
		* @param 飛ばす方向
		* @param 速さ
		* @param 発射位置
		*/
		virtual void Initialize(const D3DXVECTOR3& aShootAngle, float aPower, const D3DXVECTOR3& aShotPosition);
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		virtual void Update(float aDeltaTime);
		/**
		* @brief 3D描画をします
		*/
		virtual void Draw3D(void);

	private:
	protected:
		/**
		* @brief 当たった対象物に追尾する処理をします
		*/
		virtual void TrackingTarget(void);

		/**
		* @brief 移動処理をします
		*/
		virtual void MoveAction(float aDeltaTime);

		//矢羽からレイを飛ばす際の位置.
		D3DXVECTOR3 RayPosition;

		//当たった相手の情報.
		WPtr<GameObject> mpHitObject;
		D3DXMATRIX mLocalRotate;
		D3DXVECTOR3 mLocalPosition;
		//矢の速さ.
		float mShootPower;

		/**
		* @brief 衝突判定をします
		*/
		virtual void OnCollision(void)override;

		WaitTimeManager mWaitTimeManager;

		UPtr<class Effect> mpEffect;
	};
	
}