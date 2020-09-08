/**
* @file ArsNormalArrow.h
* @brief 通常矢クラス
*/

#pragma once
#include "ArsBaseArrow.h"

namespace Alrescha {

	class NormalArrow :public BaseArrow
	{
	public:
		NormalArrow(void);
		~NormalArrow(void)override;

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
		/**
		* @brief 衝突判定をします
		*/
		void OnCollision(void)override;
	};
}