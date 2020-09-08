/**
* @file ArsTitleScene.h
* @brief �^�C�g���V�[���N���X
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
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(void)override;
		/**
		* @brief �`������܂�
		*/
		void Draw(void)override;
	};

}