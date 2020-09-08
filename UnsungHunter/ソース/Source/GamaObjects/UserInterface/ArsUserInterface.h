/**
* @file ArsUserInterface.h
* @brief UI�Ɋւ���N���X
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
		* @brief �����������܂�
		*/
		void Initialize(void)override;

		/**
		* @brief 2D�`������܂�
		* @note Player��UI����`��
		*/
		void Draw2D(void)override;

		/**
		* @brief �N���X�w�A���k�����܂�
		*/
		void CrossHairScaleDown(void);


	private:

		D3DXVECTOR3			mScaleSize;
		Lerper<D3DXVECTOR3> mScalingLerper;

		/**
		* @brief �v���C���[�Ɋւ������\�����܂�
		*/
		void PlayerInfo(void);

		/**
		* @brief �Ə��̕\�����܂�
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