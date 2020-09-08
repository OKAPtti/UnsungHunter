/**
* @file ArsScene.h
* @brief �Q�[���V�[���N���X
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
		* @brief �X�V�����܂�
		*/
		virtual void Update(void)override;
		/**
		* @brief �`������܂�
		*/
		virtual void Draw(void)override;
		/**
		* @brief ��������܂�
		*/
		void Release(void);


	private:
		/**
		* @brief ���U���g�𐶐����܂�
		*/
		void ResultInit(const GameObjectType& aKilledCharacter);

		//���U���g���������ꂽ��.
		bool mIsResultCreated;
	};
}