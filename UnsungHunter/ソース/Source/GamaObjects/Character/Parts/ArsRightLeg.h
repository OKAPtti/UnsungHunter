/**
* @file ArsRightLeg.h
* @brief �E���N���X
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class RightLeg :public ChildGameObject
	{
	public:
		RightLeg(void);
		~RightLeg(void)override;
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

	private:
		/**
		* @brief �����A�j���[�V���������܂�
		*/
		void WalkAnimation(void);
	};

}