/**
* @file ArsLeftLeg.h
* @brief �����N���X
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class LeftLeg :public ChildGameObject
	{
	public:
		LeftLeg(void);
		~LeftLeg(void)override;
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