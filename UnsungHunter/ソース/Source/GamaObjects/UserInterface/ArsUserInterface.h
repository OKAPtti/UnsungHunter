/**
* @file ArsUserInterface.h
* @brief UIに関するクラス
*/

#pragma once

#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class Texture;
	class UserInterface :public GameObject
	{
	public:
		UserInterface(void);
		~UserInterface(void)override;

		/**
		* @brief 初期化をします
		*/
		void Initialize(void)override;

		/**
		* @brief 2D描画をします
		* @note PlayerのUI情報を描画
		*/
		void Draw2D(void)override;

		/**
		* @brief クロスヘアを縮小します
		*/
		void CrossHairScaleDown(void);


	private:

		D3DXVECTOR3			mScaleSize;
		Lerper<D3DXVECTOR3> mScalingLerper;

		/**
		* @brief プレイヤーに関する情報を表示します
		*/
		void PlayerInfo(void);

		/**
		* @brief 照準の表示します
		*/
		void ShowCrossHair(void);

		Texture* mpStaminaTexture;
		Texture* mpBarFrame;
		Texture* mpCrossHairInside;
		Texture* mpCrossHairOutside;
		Texture* mpNormalArrow;
		Texture* mpFireArrow;
		Texture* mpIconFrame;
		Texture* mpInfinityTexture;
		Texture* mpNumberTexture;
	};

}