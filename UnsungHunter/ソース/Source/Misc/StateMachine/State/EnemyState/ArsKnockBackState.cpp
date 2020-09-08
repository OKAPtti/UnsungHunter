#include "ArsKnockBackState.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"
#include "../../../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {
	KnockBackState::KnockBackState(void)
		:EnemyState(eKnockBackState)
		, mVelocity({ 0.0f,0.15f,0.2f })
	{}
	KnockBackState::~KnockBackState(void)
	{}
	void KnockBackState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);
		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }
		//ƒmƒbƒNƒoƒbƒN‚·‚é•ûŒü‚Æ‹­‚³‚ðŽæ“¾.
		mVelocity = pEnemy->GetImpactDirection();
	}
	void KnockBackState::Update(float aDeltaTime)
	{
		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }

		const D3DXVECTOR3 Gravity = { 0.0f,0.01f,0.0f };

		mVelocity -= Gravity;

		pEnemy->AddPosition(mVelocity);

		//ƒmƒbƒNƒoƒbƒN‚µ‚½‚Æ“`‚¦‚é.
		mpStateMachine->SetFlg("KnockBack", true);

		
		//’n–Ê‚©‚çŽw’è‚³‚ê‚½‚‚³“à‚É“ü‚ê‚Î‘JˆÚ.
		const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
		auto info = mpScene->RayTest(pEnemy->GetPosition(), UnderDirection, eTerrain);
		if (info.mDistance < pEnemy->GetHeight()) {
			mIsNext = true;
		}

		//HP‚Í‚OˆÈ‰º‚È‚çDeathActionState‚É‘JˆÚ.
		if (pEnemy->GetHp()->IsEmpty()) {
			mpStateMachine->ChangeState("DeathAction");
			return;
		}
	}
	void KnockBackState::OnChangeEvent(void)
	{
		mpStateMachine->SetFlg("KnockBack", false);
		mIsNext = false;
		mIsForciblyChange = false;
	}
}