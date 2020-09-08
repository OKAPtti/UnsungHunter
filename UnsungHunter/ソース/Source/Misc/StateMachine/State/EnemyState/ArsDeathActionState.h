/**
* @file ArsDeathActionState.h
* @brief ���S�����Ƃ��ɍs���X�e�[�g�N���X
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class DeathActionState :public EnemyState
	{
	public:
		DeathActionState(void);
		~DeathActionState(void)override;
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
		/**
		* @brief �A�j���[�V���������s
		*/
		void Animation(void);

		//�������������ǂ���.
		bool mIsProcessed;
		float mAnimeFrame;
	};

}