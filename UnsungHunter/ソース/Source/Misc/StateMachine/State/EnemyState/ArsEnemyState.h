/**
* @file ArsEnemyState.h
* @brief �G���g���X�e�[�g�ɕK�v�Ȃ��̂��W�߂��N���X
*/

#pragma once
#include "../ArsState.h"

namespace Alrescha {

	class EnemyState :public State
	{
	public:
		EnemyState(int aType);
		virtual ~EnemyState(void);
		/**
		* @brief �X�^�[�g���������܂�
		* @param �X�e�[�g�}�V���̃|�C���^
		* @param �Q�[���I�u�W�F�N�g
		* @param �V�[��
		* @note Update�O�ɌĂ΂�܂�
		*/
		virtual void Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene);
	

	protected:
		WPtr<class Enemy> mpEnemy;
	};

}