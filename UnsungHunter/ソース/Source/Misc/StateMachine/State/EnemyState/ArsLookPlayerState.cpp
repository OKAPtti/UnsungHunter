#include "ArsLookPlayerState.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"
#include "../../../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {
	LookPlayerState::LookPlayerState(void)
		: EnemyState(eLookPlayerState)
		, mOneSideRadView(D3DXToRadian(180))
		, mCanLookDistanceSq(30.0f * 30.0f)
	{}
	LookPlayerState::~LookPlayerState(void)
	{}
	void LookPlayerState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);
	}
	void LookPlayerState::Update(float aDeltaTime)
	{
		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return; }

		const auto PlayerPos = pPlayer->GetPosition();

		const auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		if (pEnemy->GetHp()->IsEmpty()) {
			mpStateMachine->ChangeState("DeathAction");
			return;
		}

		const auto EnemyPos = pEnemy->GetPosition();

		//対象物が視界内にいるかどうか.
		const auto TargetVec = *D3DXVec3Normalize(&D3DXVECTOR3(), &(PlayerPos - EnemyPos));
		const auto EnemyVec = *D3DXVec3Normalize(&D3DXVECTOR3(), &EnemyPos);
		float dot = D3DXVec3Dot(&EnemyVec, &TargetVec);
		const float Radian = acosf(dot);

		//視界外.
		if (isnan(Radian) || (Radian > mOneSideRadView)) {
			return;
		}

		//対象物が視認できる距離にいるかどうか.
		const float LengthSq = D3DXVec3LengthSq(&(PlayerPos - EnemyPos));

		//視認できる距離にいない.
		if (LengthSq > mCanLookDistanceSq) { return; }

		//視認できる.
		mIsNext = true;
	}
	void LookPlayerState::OnChangeEvent(void)
	{
		mIsNext = false;
		mIsForciblyChange = false;
	}
}