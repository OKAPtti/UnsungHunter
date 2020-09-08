#include "ArsState.h"

namespace Alrescha {
	State::State(int aType)
		: mType(aType)
		, mpGameObject()
		, mpStateMachine(nullptr)
		, mIsNext(false)
		, mpScene(nullptr)
		, mIsForciblyChange(false)
	{}
	State::~State(void)
	{}
	void State::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene * apScene)
	{
		mpStateMachine = apStateMachine;
		mpGameObject = apGameObject;
		mpScene = apScene;
	}
	void State::SetForciblyChange(void)
	{
		mpStateMachine->SetIsForciblyChange();
	}
}