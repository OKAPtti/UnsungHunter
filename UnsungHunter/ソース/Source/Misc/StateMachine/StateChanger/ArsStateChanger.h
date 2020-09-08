/**
* @file ArsStateChanger.h
* @brief �X�e�[�g��ύX����N���X
*/

#pragma once

namespace Alrescha {
	class State;
	class Scene;
	class StateChanger
	{
	public:
		StateChanger(void) = default;
		/**
		* @param �o�^����X�e�[�g
		* @param �X�e�[�g��
		*/
		StateChanger(const SPtr<State> apState, const std::string& aNextStateName);
		StateChanger(const SPtr<State> apState);
		~StateChanger(void);
		/**
		* @brief �X�^�[�g���������܂�
		* @param �X�e�[�g�}�V���̃|�C���^
		* @param �Q�[���I�u�W�F�N�g
		* @param �V�[��
		* @note Update�O�ɌĂ΂�܂�
		*/
		virtual void Start(StateMachine*apStateMachine,WPtr<class GameObject> apGameObject,Scene*apScene);
		
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		virtual void Update(float aDeltaTime);
	
		/**
		* @brief ���̏�ԂɑJ�ڂł��邩�ǂ�����Ԃ��܂�
		*/
		bool CanNextState(void);

		/**
		* @brief ���ɑJ�ڂ���X�e�[�g�̖��O��o�^���܂�
		* @param �X�e�[�g�̖��O
		*/
		void SetNextRegisterName(const std::string& aNextName) { mNextRegisterName = aNextName; }
		
		/**
		* @brief ���ɑJ�ڂ��閼�O��Ԃ��܂�
		*/
		const std::string& GetNextRegisterName(void)const { return mNextRegisterName; }

		/**
		* @brief �X�e�[�g�̃^�C�v��Ԃ��܂�
		*/
		const int GetStateType(void)const;

		/**
		* @brief �����I�ɃX�e�[�g��ύX
		* @note ChangeState�������̂ݎ��s����܂�
		*/
		void ForciblyChange(void);
	private:
		SPtr<State> mpState;

		//���ɍs����Ԃ�o�^�������O.
		std::string mNextRegisterName;

	protected:
	};

}