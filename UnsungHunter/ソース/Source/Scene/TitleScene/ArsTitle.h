/**
* @file ArsTitle.h
* @brief タイトルクラス
*/

#pragma once
#include "../../GamaObjects/BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class Texture;
	class Title :public GameObject
	{
	public:
		Title(void);
		virtual ~Title(void)override final;
		/**
		* @brief 初期化をします
		*/
		void Initialize(void)override;
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(float aDeltaTime)override;
		/**
		* @brief 2D描画をします
		*/
		void Draw2D(void)override;
	private:
		Texture* mpSubTexture;
		Texture* mpBackGroundTexture;
		int mAlphaValue;
		bool mSwitchFlg;
	};

}