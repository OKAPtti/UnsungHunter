/**
* @file ArsLeftArm.h
* @brief 左腕クラス
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class LeftArm :public ChildGameObject
	{
	public:
		LeftArm(void);
		~LeftArm(void)override;
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

	private:
		/**
		* @brief 歩きアニメーションをします
		*/
		void WalkAnimation(void);
		/**
		* @brief 弓を構えるアニメーションをします
		*/
		void HoldingBow(void);
	};

}