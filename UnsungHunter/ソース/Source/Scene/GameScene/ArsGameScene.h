/**
* @file ArsScene.h
* @brief ゲームシーンクラス
*/

#pragma once

#include "../BaseScene/ArsScene.h"
#include "../../GamaObjects/ArsGameObjectType.h"

namespace Alrescha {
	class GameScene:public Scene
	{
	public:
		GameScene(GameFrame* apGameFrame);
		virtual~GameScene(void)override final;

		/**
		* @brief 更新をします
		*/
		virtual void Update(void)override;
		/**
		* @brief 描画をします
		*/
		virtual void Draw(void)override;
		/**
		* @brief 解放をします
		*/
		void Release(void);


	private:
		/**
		* @brief リザルトを生成します
		*/
		void ResultInit(const GameObjectType& aKilledCharacter);

		//リザルトが生成されたか.
		bool mIsResultCreated;
	};
}