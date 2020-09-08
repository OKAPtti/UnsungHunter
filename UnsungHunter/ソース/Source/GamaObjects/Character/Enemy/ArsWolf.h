/**
* @file ArsWolf.h
* @brief �I�I�J�~�N���X
*/

#pragma once
#include "ArsEnemy.h"

namespace Alrescha {
	class Wolf :public Enemy
	{
	public:
		Wolf(void);
		~Wolf(void)override final;

		/**
		* @brief �����������܂�
		*/
		void Initialize(void)override final;

		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime)override final;
		/**
		* @brief 3D�`������܂�
		*/
		void Draw3D(void)override final;
		/**
		* @brief State����]�X�e�[�g�ɂ��܂�
		* @note �{�X���珢�����ꂽ�Ƃ��Ɏg�p���܂�
		*/
		void ChangeRotateState(void);
	private:
		/**
		* @brief �Փ˔�������܂�
		*/
		void OnCollision(void)override;
	};
}