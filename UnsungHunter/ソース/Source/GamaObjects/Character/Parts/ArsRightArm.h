/**
* @file ArsRightArm.h
* @brief 右腕クラス
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class RightArm :public ChildGameObject
	{
	public:
		RightArm(void);
		~RightArm(void)override;

		/**
		* @brief 初期化をします
		* @param モデル名
		*/
		void Initialize(const std::string& aModelName)override;
		/**
		* @brief 行列を更新
		* @param 親の行列
		* @param デルタタイム
		*/
		void UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)override;
		/**
		* @brief 描画をします
		* @param アルファ値
		* @note 引数無しの場合は1.0f
		*/
		void Draw(float aAlphaValue)override;

		/**
		* @brief 弓を構えるアニメーションをします
		*/
		void HoldingBow(void);
	private:
		/**
		* @brief 歩きアニメーションをします
		*/
		void WalkAnimation(void);

	};

}