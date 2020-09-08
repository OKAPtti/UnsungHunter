/**
* @file ArsState.h
* @brief �X�e�[�g�ɕK�v�Ȃ��̂��W�߂��N���X
*/

#pragma once

#include "ArsStateType.h"

namespace Alrescha {
	class Scene;
	class State : public std::enable_shared_from_this<State>
	{
	public:
		State(int aType);
		virtual ~State(void);

		/**
		* @brief �X�^�[�g���������܂�
		* @param �X�e�[�g�}�V���̃|�C���^
		* @param �Q�[���I�u�W�F�N�g
		* @param �V�[��
		* @note Update�O�ɌĂ΂�܂�
		*/
		virtual void Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene);
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		virtual void Update(float aDeltaTime) {}

		/**
		* @brief �C�x���g�ύX���������܂�
		* @note �X�e�[�g���؂�ւ��O�Ɏ��s����܂�
		*/
		virtual void OnChangeEvent(void) {}
		/**
		* @brief �X�e�[�g��؂�ւ���t���O��Ԃ��܂�
		*/
		bool GetIsNext(void)const { return mIsNext; }
		/**
		* @brief �X�e�[�g�������I�����I�؂�ւ���t���O��Ԃ��܂�
		*/
		bool GetIsForciblyChange(void) { return mIsForciblyChange; }

		/**
		* @brief ���g�̃^�C�v��Ԃ��܂�
		*/
		const int GetType(void)const { return mType; }

		/**
		* @brief ���g��(static)�L���X�g���܂�
		* @param �^�C�v
		* @note ���̃L���X�g���g���Ƃ��͈��������܂��Ă��鎞����.
		*/
		template<class T>
		SPtr<T> Cast(int aType);

		/**
		* @brief �����I�ɃX�e�[�g��ύX�����܂�
		*/
		void SetForciblyChange(void);
	private:
		int mType;
	protected:
		WPtr<class GameObject> mpGameObject;

		StateMachine* mpStateMachine;

		Scene* mpScene;

		//���̏�Ԃɍs�����߂̏���.
		//��������������Ă����true�ɂ���.
		bool mIsNext;
		//�����I�ɏ�Ԃ�ύX���邩.
		bool mIsForciblyChange;
	};

	template<class T>
	inline SPtr<T> State::Cast(int aType)
	{
		if (GetType() == aType) {
			return std::static_pointer_cast<T>(shared_from_this());
		}
		return nullptr;
	}

}