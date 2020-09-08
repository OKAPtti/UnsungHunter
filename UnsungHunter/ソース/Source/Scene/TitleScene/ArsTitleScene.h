/**
* @file ArsTitleScene.h
* @brief タイトルシーンクラス
*/

#pragma once
#include "../BaseScene/ArsScene.h"

namespace Alrescha {

	class TitleScene :public Scene
	{
	public:
		TitleScene(GameFrame* apGameFrame);
		virtual ~TitleScene(void);

		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(void)override;
		/**
		* @brief 描画をします
		*/
		void Draw(void)override;
	};

}