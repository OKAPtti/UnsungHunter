/**
* @file ArsQuiver.h
* @brief 矢の入れ物クラス
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Quiver :public GameObject
	{
	public:
		Quiver(void);
		~Quiver(void)override;
		/**
		* @brief 初期化をします
		*/
		void Initialize(void)override final;
		/**
		* @brief 更新処理をします
		* @param 親の行列
		* @param デルタタイム
		*/
		void Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime);
		/**
		* @brief 3D描画をします
		*/
		void Draw3D(void)override final;

	private:
	};

}