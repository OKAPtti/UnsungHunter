/**
* @file ArsLeftArm.h
* @brief ���r�N���X
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class LeftArm :public ChildGameObject
	{
	public:
		LeftArm(void);
		~LeftArm(void)override;
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
		/**
		* @brief �|���\����A�j���[�V���������܂�
		*/
		void HoldingBow(void);
	};

}