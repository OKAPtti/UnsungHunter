/**
* @file ArsStateMachine.h
* @brief �X�e�[�g�}�V���N���X
*/

#pragma once

namespace Alrescha {
	class State;
	class StateChanger;
	class GameObject;
	class Scene;
	class StateMachine
	{
	public:
		StateMachine(void);
		~StateMachine(void);
		
		//����:�Ώ�,�V�[��.
		/**
		* @brief �Z�b�g�A�b�v�����܂�
		* @param �Q�[���I�u�W�F�N�g
		* @param �V�[��
		*/
		void SetUp(WPtr<GameObject> apGameObject, Scene* apScene);

		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime);

		/**
		* @brief State��o�^���܂�
		* @param �o�^��
		* @param StateChanger
		*/
		void Register(const std::string& aRegisterName,const SPtr<StateChanger> apStateChanger);

		/**
		* @brief ���߂̏�Ԃ��Z�b�g���܂�
		* @param �o�^���ꂽ���O
		*/
		void SetStartState(const std::string& aStartRegisterName);


		/**
		* @brief StateType��Ԃ��܂�
		*/
		const int GetStateType(void)const;

		/**
		* @brief �t���O��Ԃ��܂�
		* @param �t���O��
		*/
		bool GetFlg(const std::string& aFlgName)const;
		
		/**
		* @brief �t���O�Z�b�g���܂�
		* @param �t���O��
		* @param �t���O
		*/
		void SetFlg(const std::string& aFlgName, bool aFlg) { mFlgMap[aFlgName] = aFlg; }
		
		/**
		* @brief �V�[�����Z�b�g���܂�
		* @param �V�[��
		*/
		void SetScene(Scene* apScene) { mpScene = apScene; }
	
		/**
		* @brief mIsForciblyChange��true�ɂ��܂�
		*/
		void SetIsForciblyChange(void) { mIsForciblyChange = true; }

		/**
		* @brief �X�e�[�g��ύX���܂�
		* @param �ύX�������X�e�[�g�̖��O
		*/
		void ChangeState(const std::string& aChangeState);
	private:
		WPtr<GameObject> mpGameObject;
		Scene* mpScene;

		std::unordered_map<std::string, SPtr<StateChanger>> mStateMap;

		//�X�e�[�g���瑗����t���O
		std::unordered_map<std::string, bool> mFlgMap;

		//���݂̃X�e�[�g.
		SPtr<StateChanger> mpCurrentState;

		//�����I�ɃX�e�[�g��ύX���邩.
		bool mIsForciblyChange;
	};

}