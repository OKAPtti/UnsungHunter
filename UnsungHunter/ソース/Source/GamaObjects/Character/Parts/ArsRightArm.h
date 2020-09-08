/**
* @file ArsRightArm.h
* @brief �E�r�N���X
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class RightArm :public ChildGameObject
	{
	public:
		RightArm(void);
		~RightArm(void)override;

		/**
		* @brief �����������܂�
		* @param ���f����
		*/
		void Initialize(const std::string& aModelName)override;
		/**
		* @brief �s����X�V
		* @param �e�̍s��
		* @param �f���^�^�C��
		*/
		void UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)override;
		/**
		* @brief �`������܂�
		* @param �A���t�@�l
		* @note ���������̏ꍇ��1.0f
		*/
		void Draw(float aAlphaValue)override;

		/**
		* @brief �|���\����A�j���[�V���������܂�
		*/
		void HoldingBow(void);
	private:
		/**
		* @brief �����A�j���[�V���������܂�
		*/
		void WalkAnimation(void);

	};

}