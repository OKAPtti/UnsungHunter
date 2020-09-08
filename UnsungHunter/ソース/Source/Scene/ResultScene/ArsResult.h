/**
* @file ArsResult.h
* @brief リザルトクラス
*/

#pragma once
#include "../../GamaObjects/BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class Texture;
	class Result :public GameObject
	{
	public:
		Result(void);
		~Result(void)override;

		/**
		* @brief 初期化をします
		* @param キルされたキャラクタータイプ
		* @note キャラクタータイプによって処理が変わります
		*/
		void Initialize(const GameObjectType& aKilledCharacter);

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
		int mAlphaValue;
		bool mSwitchFlg;
	};
}