/**
* @file ArsResult.h
* @brief ���U���g�N���X
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
		* @brief �����������܂�
		* @param �L�����ꂽ�L�����N�^�[�^�C�v
		* @note �L�����N�^�[�^�C�v�ɂ���ď������ς��܂�
		*/
		void Initialize(const GameObjectType& aKilledCharacter);

		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime)override;
		/**
		* @brief 2D�`������܂�
		*/
		void Draw2D(void)override;

	private:
		Texture* mpSubTexture;
		int mAlphaValue;
		bool mSwitchFlg;
	};
}