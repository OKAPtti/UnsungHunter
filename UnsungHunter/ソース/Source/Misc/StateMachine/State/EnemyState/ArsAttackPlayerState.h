/**
* @file ArsAttackPlayerState.h
* @brief �v���C���[�Ɍ������čU������X�e�[�g�N���X
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class AttackPlayerState :public EnemyState
	{
	public:
		AttackPlayerState(void);
		~AttackPlayerState(void)override;

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
		//�ړ�����͂ƌ���.
		D3DXVECTOR3 mVelocity;

		//�W�����v�U��.
		void JumpAttack(float aDeltaTime);
	};
}