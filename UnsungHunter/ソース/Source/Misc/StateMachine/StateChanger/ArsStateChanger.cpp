#include "ArsStateChanger.h"
#include "../State/ArsState.h"

namespace Alrescha {
	StateChanger::StateChanger(const SPtr<State> apState, const std::string& aNextStateName)
		: mpState(apState)
		, mNextRegisterName(aNextStateName)
	{}
	StateChanger::StateChanger(const SPtr<State> apState)
		: mpState(apState)
		, mNextRegisterName()
	{}
	StateChanger::~StateChanger(void)
	{
	}
	void StateChanger::Start(StateMachine* apStateMachine,WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		mpState->Start(apStateMachine,apGameObject,apScene);
	}
	void StateChanger::Update(float aDeltaTime)
	{
		mpState->Update(aDeltaTime);

		//強制的に遷移を変更させるか.
		if (mpState->GetIsForciblyChange()) {
			mpState->SetForciblyChange();
			mpState->OnChangeEvent();
		}
	}
	bool StateChanger::CanNextState(void)
	{
		//遷移が可能ならば.
		if (mpState->GetIsNext()) {
			//遷移する.
			mpState->OnChangeEvent();
			return true;
		}
		//遷移しない.
		return false;
	}
	const int StateChanger::GetStateType(void) const
	{
		return mpState->GetType();
	}
	void StateChanger::ForciblyChange(void)
	{
		mpState->OnChangeEvent();
	}
}