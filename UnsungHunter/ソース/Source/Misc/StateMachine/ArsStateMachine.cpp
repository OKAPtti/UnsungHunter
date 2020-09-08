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

		//ŽŸ‚Ìó‘Ô‚É‘JˆÚ‰Â”\‚Å‚ ‚ê‚Î.
		if (mpCurrentState->CanNextState()) {
			//ŽŸ‚Ìó‘Ô‚Ì–¼‘O‚ª“o˜^‚³‚ê‚Ä‚¢‚é‚©.
			auto it = mStateMap.find(mpCurrentState->GetNextRegisterName());
			//“o˜^‚³‚ê‚Ä‚¢‚È‚¢.
			if (it == mStateMap.end()) { return; }

			//“o˜^‚³‚ê‚Ä‚¢‚é.
			//ŽŸ‚Ìó‘Ô‚É‘JˆÚ.
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

		//ŠJŽnŽž‚Ìó‘Ô‚ðƒZƒbƒg.
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
		//Œ©‚Â‚©‚ê‚ÎƒL[‚É‘Î‚·‚é’l‚ð•Ô‚·.
		if (it == mFlgMap.end()) {
			return false;
		}
		return it->second;
	}
	void StateMachine::ChangeState(const std::string& aChangeState)
	{
		auto it = mStateMap.find(aChangeState);
		if (it == mStateMap.end()) {
			_ASSERT_EXPR(false, L"“n‚³‚ê‚½State–¼‚Í“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñ");
		}

		//Œ»Ý‚ÌState‚ÌI—¹ˆ—‚ð‚·‚é.
		mpCurrentState->ForciblyChange();

		//State‚ð•ÏX.
		mpCurrentState = it->second;
		mpCurrentState->Start(this, mpGameObject, mpScene);
	}
}
