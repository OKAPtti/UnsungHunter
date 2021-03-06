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
		//ノックバックする方向と強さを取得.
		mVelocity = pEnemy->GetImpactDirection();
	}
	void KnockBackState::Update(float aDeltaTime)
	{
		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }

		const D3DXVECTOR3 Gravity = { 0.0f,0.01f,0.0f };

		mVelocity -= Gravity;

		pEnemy->AddPosition(mVelocity);

		//ノックバックしたと伝える.
		mpStateMachine->SetFlg("KnockBack", true);

		
		//地面から指定された高さ内に入れば遷移.
		const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
		auto info = mpScene->RayTest(pEnemy->GetPosition(), UnderDirection, eTerrain);
		if (info.mDistance < pEnemy->GetHeight()) {
			mIsNext = true;
		}

		//HPは０以下ならDeathActionStateに遷移.
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