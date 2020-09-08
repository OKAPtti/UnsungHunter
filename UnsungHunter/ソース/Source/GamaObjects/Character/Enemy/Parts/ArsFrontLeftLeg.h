/**
* @file ArsFrontLeftLeg.h
* @brief オオカミの左前脚を管理するクラス
*/

#pragma once
#include "../../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class FrontLeftLeg :public ChildGameObject
	{
	public:
		FrontLeftLeg(void);
		~FrontLeftLeg(void)override;

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
		* @brief 赤色に描画をします
		*/
		void DrawRed(void)override;

		/**
		* @brief 歩きアニメーションをします
		* @note クォータニオンを使った補間
		*/
		void WalkAnimation(void)override;

	private:
		bool mIsReverse;
	};

}