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

		//次の状態に遷移可能であれば.
		if (mpCurrentState->CanNextState()) {
			//次の状態の名前が登録されているか.
			auto it = mStateMap.find(mpCurrentState->GetNextRegisterName());
			//登録されていない.
			if (it == mStateMap.end()) { return; }

			//登録されている.
			//次の状態に遷移.
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

		//開始時の状態をセット.
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
		//見つかればキーに対する値を返す.
		if (it == mFlgMap.end()) {
			return false;
		}
		return it->second;
	}
	void StateMachine::ChangeState(const std::string& aChangeState)
	{
		auto it = mStateMap.find(aChangeState);
		if (it == mStateMap.end()) {
			_ASSERT_EXPR(false, L"渡されたState名は登録されていません");
		}

		//現在のStateの終了処理をする.
		mpCurrentState->ForciblyChange();

		//Stateを変更.
		mpCurrentState = it->second;
		mpCurrentState->Start(this, mpGameObject, mpScene);
	}
}
