/**
* @file ArsFireArrow.h
* @brief 炎の矢クラス
*/

#pragma once
#include "ArsBaseArrow.h"

namespace Alrescha {

	class FireArrow :public BaseArrow
	{
	public:
		FireArrow(void);
		~FireArrow(void)override;

		/**
		* @brief 初期化をします
		* @param 飛ばす方向
		* @param 速さ
		* @param 発射位置
		*/
		void Initialize(const D3DXVECTOR3& aShootAngle, float aPower, const D3DXVECTOR3& aShotPosition)override;
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(float aDeltaTime)override;
		/**
		* @brief 3D描画をします
		*/
		void Draw3D(void)override;
	private:
		//炎を生成したか.
		bool mIsCreateFireWall;

		/**
		* @brief 衝突判定をします
		*/
		void OnCollision(void)override;

		//継続ダメージ.
		WaitTimeManager mWaitTimeManager;
	};

}