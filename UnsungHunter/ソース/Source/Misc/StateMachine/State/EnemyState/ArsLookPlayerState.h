/**
* @file ArsLookPlayerState.h
* @brief �v���C���[�����F�ł��邩�X�e�[�g�N���X
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class LookPlayerState :public EnemyState
	{
	public:
		LookPlayerState(void);
		~LookPlayerState(void)override;
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

		//����̕Б��͈̔�(���W�A���p)
		/**
		* @brief ����̕Б��͈̔�(���W�A���p)��ݒ肵�܂�
		* @param ����p
		*/
		void SetOneSideView(float aRadianAngle) { mOneSideRadView = aRadianAngle; }
		
		/**
		* @brief ���F�ł��鋗����ݒ肵�܂�
		* @param ����
		*/
		void SetCanLookDistance(float aDistance) { mCanLookDistanceSq = aDistance * aDistance; }

	private:
		float mOneSideRadView;
		float mCanLookDistanceSq;
	};

}