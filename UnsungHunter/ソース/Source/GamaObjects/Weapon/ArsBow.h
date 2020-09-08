/**
* @file ArsBow.h
* @brief 弓クラス
*/

#pragma once
#include "ArsWeapon.h"
#include "../Character/ArsArrowType.h"

namespace Alrescha {

	class Bow :public Weapon
	{
	public:
		Bow(void);
		~Bow(void)override;
		/**
		* @brief 初期化をします
		*/
		void Initialize(void)override;
		/**
		* @brief 更新処理をします
		* @param 親の行列
		* @param デルタタイム
		*/
		void Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime);
		/**
		* @brief 3D描画をします
		*/
		void Draw3D(void)override;
		/**
		* @brief 矢を生成します
		*/
		void Shot(const ArrowType& aArrowType);

	private:
		D3DXMATRIX mShiftMatrix;
	};

}