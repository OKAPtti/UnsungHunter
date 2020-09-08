/**
* @file ArsFrontLeftLeg.h
* @brief �I�I�J�~�̍��O�r���Ǘ�����N���X
*/

#pragma once
#include "../../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class FrontLeftLeg :public ChildGameObject
	{
	public:
		FrontLeftLeg(void);
		~FrontLeftLeg(void)override;

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
		* @brief �ԐF�ɕ`������܂�
		*/
		void DrawRed(void)override;

		/**
		* @brief �����A�j���[�V���������܂�
		* @note �N�H�[�^�j�I�����g�������
		*/
		void WalkAnimation(void)override;

	private:
		bool mIsReverse;
	};

}