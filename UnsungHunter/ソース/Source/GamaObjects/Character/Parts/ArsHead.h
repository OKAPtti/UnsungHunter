/**
* @file ArsHead.h
* @brief 頭クラス
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class Head :public ChildGameObject
	{
	public:
		Head(void);
		~Head(void)override;
		/**
		* @brief 初期化をします
		* @param モデル名
		*/
		virtual void Initialize(const std::string& aModelName);
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
		* @brief 赤色に描画をします
		*/
		void DrawRed(void)override;
	};

}