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
		//�U�����󂯂Ă���΋����I��KnockBackState�ɑJ��.
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		//HP�͂O�ȉ��Ȃ�DeathActionState�ɑJ��.
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