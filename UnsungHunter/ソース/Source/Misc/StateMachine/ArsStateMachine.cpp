#include "ArsStateMachine.h"
#include "StateChanger/ArsStateChanger.h"

namespace Alrescha {
	StateMachine::StateMachine(void)
		: mStateMap()
		, mpGameObject()
		, mpScene(nullptr)
		, mFlgMap()
		, mpCurrentState(nullptr)
		, mIsForciblyChange(false)
	{}
	StateMachine::~StateMachine(void)
	{}
	void StateMachine::SetUp(WPtr<GameObject> apGameObject, Scene * apScene)
	{
		mpGameObject = apGameObject;
		mpScene = apScene;
	}
	void StateMachine::Update(float aDeltaTime)
	{
		mpCurrentState->Update(aDeltaTime);

		//���̏�ԂɑJ�ډ\�ł����.
		if (mpCurrentState->CanNextState()) {
			//���̏�Ԃ̖��O���o�^����Ă��邩.
			auto it = mStateMap.find(mpCurrentState->GetNextRegisterName());
			//�o�^����Ă��Ȃ�.
			if (it == mStateMap.end()) { return; }

			//�o�^����Ă���.
			//���̏�ԂɑJ��.
			mpCurrentState = it->second;
			mpCurrentState->Start(this,mpGameObject, mpScene);
		}
	}
	void StateMachine::Register(const std::string& aRegisterName, const SPtr<StateChanger> apStateChanger)
	{
		mStateMap.insert(std::make_pair(aRegisterName, apStateChanger));

	}
	void StateMachine::SetStartState(const std::string& aStartRegisterName)
	{
		auto it = mStateMap.find(aStartRegisterName);
		if (it == mStateMap.end()) { return; }

		//�J�n���̏�Ԃ��Z�b�g.
		mpCurrentState = it->second;
		mpCurrentState->Start(this,mpGameObject,mpScene);
	}
	const int StateMachine::GetStateType(void) const
	{
		return mpCurrentState->GetStateType();
	}
	bool StateMachine::GetFlg(const std::string& aFlgName) const
	{
		auto it = mFlgMap.find(aFlgName);
		//������΃L�[�ɑ΂���l��Ԃ�.
		if (it == mFlgMap.end()) {
			return false;
		}
		return it->second;
	}
	void StateMachine::ChangeState(const std::string& aChangeState)
	{
		auto it = mStateMap.find(aChangeState);
		if (it == mStateMap.end()) {
			_ASSERT_EXPR(false, L"�n���ꂽState���͓o�^����Ă��܂���");
		}

		//���݂�State�̏I������������.
		mpCurrentState->ForciblyChange();

		//State��ύX.
		mpCurrentState = it->second;
		mpCurrentState->Start(this, mpGameObject, mpScene);
	}
}
