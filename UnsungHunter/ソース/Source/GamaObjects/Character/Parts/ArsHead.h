/**
* @file ArsHead.h
* @brief ���N���X
*/

#pragma once
#include "../../BaseGameObjects/ArsChildGameObject.h"

namespace Alrescha {

	class Head :public ChildGameObject
	{
	public:
		Head(void);
		~Head(void)override;
		/**
		* @brief �����������܂�
		* @param ���f����
		*/
		virtual void Initialize(const std::string& aModelName);
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
	};

}