#include "ArsFreeMoveState.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"
#include "../../../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {

	FreeMoveState::FreeMoveState(void)
		: EnemyState(eFreeMoveState)
	{}

	FreeMoveState::~FreeMoveState(void)
	{}

	void FreeMoveState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);
	}

	void FreeMoveState::Update(float aDeltaTime)
	{
		auto pEnemy = mpEnemy.lock();
		//UŒ‚‚ðŽó‚¯‚Ä‚¢‚ê‚Î‹­§“I‚ÉKnockBackState‚É‘JˆÚ.
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		//HP‚Í‚OˆÈ‰º‚È‚çDeathActionState‚É‘JˆÚ.
		if (pEnemy->GetHp()->IsEmpty()) {
			mpStateMachine->ChangeState("DeathAction");
			return;
		}

		mIsNext = true;
	}
	void FreeMoveState::OnChangeEvent(void)
	{
		mIsNext = false;
		mIsForciblyChange = false;
	}
}