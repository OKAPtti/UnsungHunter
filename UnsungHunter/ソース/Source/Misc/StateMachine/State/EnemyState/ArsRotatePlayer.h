/**
* @file ArsRotatePlayer.h
* @brief �v���C���[�̕����Ɍ����X�e�[�g�N���X
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class RotatePlayer :public EnemyState
	{
	public:
		RotatePlayer(void);
		~RotatePlayer(void)override;
		/**
		* @brief �X�^�[�g���������܂�
		* @param �X�e�[�g�}�V���̃|�C���^
		* @param �Q�[���I�u�W�F�N�g
		* @param �V�[��
		* @note Update�O�ɌĂ΂�܂�
		*/
		void Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)override;
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime)override;
		/**
		* @brief �C�x���g�ύX���������܂�
		* @note �X�e�[�g���؂�ւ��O�Ɏ��s����܂�
		*/
		void OnChangeEvent(void)override;

	private:
	};

}