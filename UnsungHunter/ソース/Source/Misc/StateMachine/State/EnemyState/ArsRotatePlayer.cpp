#include "ArsRotatePlayer.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"
#include "../../../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {
	RotatePlayer::RotatePlayer(void)
		: EnemyState(eRotatePlayerState)
	{}
	RotatePlayer::~RotatePlayer(void)
	{
	}
	void RotatePlayer::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);
	}
	void RotatePlayer::Update(float aDeltaTimevoid)
	{
		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return; }

		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }
		//攻撃を受けていれば強制的にKnockBackStateに遷移.
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		const auto EnemyMatrix = pEnemy->GetMatrix();
		const auto PlayerPos = pPlayer->GetPosition();

		//敵の向き
		D3DXVECTOR3 forward;
		D3DXVec3TransformNormal(&forward, &D3DXVECTOR3{ 0.0f,0.0f,1.0f }, &EnemyMatrix);
		D3DXVec3Normalize(&forward, &forward);

		//基準ベクトルからの角度
		float baseAngle = 0;
		baseAngle = D3DXVec3Dot(&forward, &D3DXVECTOR3{ 0.0f,0.0f,1.0f });
		baseAngle = D3DXToDegree(acos(baseAngle));
		if (forward.x < 0.0f) { baseAngle *= -1.0f; }

		//プレイヤーへの角度
		D3DXVECTOR3 pos{ pEnemy->GetPosition() - PlayerPos };
		D3DXVec3Normalize(&pos, &pos);

		float tRot = D3DXVec3Dot(&D3DXVECTOR3{ 0.0f,0.0f,1.0f }, &pos);
		tRot = D3DXToDegree(acos(tRot));
		if (pos.x < 0.0f) { tRot *= -1.0f; }

		constexpr float MaxAngle = 180.0f;
		constexpr float Inversion = 360.0f;
		float angle = tRot - baseAngle;
		if (angle > MaxAngle) { angle -= Inversion; }
		else if (angle < -MaxAngle) { angle += Inversion; }

		constexpr float RotSpeed = 5.0f;
		float rotY = 0;
		if (angle < 0) {
			//痙攣を防ぐ.
			if (angle > -RotSpeed) {
				rotY = angle;
				if (pPlayer->GetHp()->IsEmpty()) { return; }
				mIsNext = true;
			}
			else { rotY = -RotSpeed; }
		}
		else if (angle >= 0) {
			//痙攣を防ぐ.
			if (angle < RotSpeed) {
				rotY = angle;
				if (pPlayer->GetHp()->IsEmpty()) { return; }
				mIsNext = true;
			}
			else { rotY = RotSpeed; }
		}
		//歩いたことを伝える.
		if (!pPlayer->GetHp()->IsEmpty()) { mpStateMachine->SetFlg("Move", true); }
		else { mpStateMachine->SetFlg("Move", false); }

		//HPは0下ならDeathActionStateに遷移.
		if (pEnemy->GetHp()->IsEmpty()) { mpStateMachine->ChangeState("DeathAction"); }

		D3DXMATRIX rotate;
		D3DXMatrixRotationY(&rotate, D3DXToRadian(rotY));
		pEnemy->AddRotation(rotate);
	}
	void RotatePlayer::OnChangeEvent(void)
	{
		mpStateMachine->SetFlg("Move", false);
		mIsNext = false;
		mIsForciblyChange = false;
	}
}