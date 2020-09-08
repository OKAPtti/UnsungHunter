/**
* @file ArsGoToPlayerState.h
* @brief �v���C���[�Ɍ������Ĉړ�����X�e�[�g�N���X
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class GoToPlayerState :public EnemyState
	{
	public:
		GoToPlayerState(void);
		~GoToPlayerState(void)override;
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

		/**
		* @brief ���̏�ԂɑJ�ڂ��鋗����ݒ肵�܂�
		* @param ����
		*/
		void SetNextStateDistance(float aLength) { mNextStateLength = aLength; }

	private:
		float mNextStateLength;
		D3DXVECTOR3 mPlayerPos;

		/**
		* @brief �v���C���[�Ƃ̊Ԃɉ��̕ǂ����邩�ǂ������ׂ܂�
		* @note �������ꍇ��true
		*/
		bool IsThereFireWallInFront(void);
	};

}