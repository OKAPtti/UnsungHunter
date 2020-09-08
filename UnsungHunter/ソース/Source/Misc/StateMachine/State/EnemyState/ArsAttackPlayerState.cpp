#include "ArsAttackPlayerState.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"
#include "../../../../Scene/BaseScene/ArsScene.h"
#include "../../../../GamaObjects/Stage/ArsWall.h"

namespace Alrescha {
	AttackPlayerState::AttackPlayerState(void)
		: EnemyState(eAttackPlayerState)
		, mVelocity({ 0.0f,0.0f,0.0f })
	{}
	AttackPlayerState::~AttackPlayerState(void)
	{}
	void AttackPlayerState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);
		auto pEnemy = mpEnemy.lock();
		//ƒ^ƒCƒv‚É‚æ‚Á‚ÄƒWƒƒƒ“ƒvUŒ‚‚Ì”’l‚ð•ÏX.
		if (pEnemy->GetType() == eWolf) {
			mVelocity = { 0.0f,0.15f,-0.2f };
		}
		else if (pEnemy->GetType() == eBoss)
		{
			mVelocity = { 0.0f,0.15f,-0.25f };
		}
	}
	void AttackPlayerState::Update(float aDeltaTime)
	{
		JumpAttack(aDeltaTime);
	}
	void AttackPlayerState::OnChangeEvent(void)
	{
		mpStateMachine->SetFlg("Attack", false);
		mIsNext = false;
		mIsForciblyChange = false;
	}
	void AttackPlayerState::JumpAttack(float aDeltaTime)
	{
		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return; }
		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		const D3DXVECTOR3 Gravity = { 0.0f,0.01f,0.0f };

		mVelocity -= Gravity;

		D3DXVECTOR3 vel;
		D3DXVec3TransformNormal(&vel, &mVelocity, &pEnemy->GetRotateMatrix());

		//UŒ‚‚µ‚½‚Æ“`‚¦‚é.
		mpStateMachine->SetFlg("Attack", true);

		D3DXVECTOR3 front{ 0.0f,0.0f,-1.0f };

		D3DXVec3TransformNormal(&front, &front, &pEnemy->GetRotateMatrix());

		InputRay input;
		input.mRayPos = pEnemy->GetPosition();
		input.mRayDir = front;
		OutputRay output;

		auto pObj = mpScene->FindGameObject(eWall);
		if (pObj) {
			auto pWall = pObj->Cast<Wall>(eWall);
			if (pWall) {
				pWall->HitCheckRay(input, output);
				pEnemy->AddPosition(output.mForceOut);
			}
		}

		pEnemy->AddPosition(vel);

		auto info = mpScene->RayTest(pEnemy->GetPosition(), { 0.0f,-1.0f,0.0f }, eTerrain);
		if (info.mDistance <= pEnemy->GetHeight()) {
			mIsNext = true;
		}

		auto pWolf = pEnemy->Cast<Wolf>(eWolf);
		if (!pWolf) { return; }
		//HP‚Í‚OˆÈ‰º‚È‚çDeathActionState‚É‘JˆÚ.
		if (pWolf->GetHp()->IsEmpty()) {
			mpStateMachine->ChangeState("DeathAction");
			return;
		}
	}
}