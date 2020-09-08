/**
* @file ArsTitle.h
* @brief �^�C�g���N���X
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
		* @brief �����������܂�
		*/
		void Initialize(void)override;
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
		Texture* mpBackGroundTexture;
		int mAlphaValue;
		bool mSwitchFlg;
	};

}