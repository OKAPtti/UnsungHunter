#include "ArsStopMoveState.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"
#include "../../../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {
	StopMoveState::StopMoveState(void)
		: EnemyState(eStopMoveState)
	{}
	StopMoveState::~StopMoveState(void)
	{}
	void StopMoveState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);
	}
	void StopMoveState::Update(float aDeltaTime)
	{

		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }
		//攻撃を受けていれば強制的にKnockBackStateに遷移.
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		//HPは0以下ならDeathActionStateに遷移.
		if (pEnemy->GetHp()->IsEmpty()) { mpStateMachine->ChangeState("DeathAction"); }

		//移動していないことを伝える.
		mpStateMachine->SetFlg("Move", false);

		//正面に炎があるかどうか.
		IsThereFireWallInFront();
	}

	void StopMoveState::OnChangeEvent(void)
	{
		mIsNext = false;
	}
	void StopMoveState::IsThereFireWallInFront(void)
	{
		std::list<SPtr<GameObject>> pObjList = mpScene->FindGameObjectList(eFireWallFragment);

		//空かどうか.
		if (pObjList.empty()) {
			//炎がなければ遷移.
			mIsNext = true;
		}

		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }

		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return; }

		D3DXVECTOR3 targetVec{ pPlayer->GetPosition() - pEnemy->GetPosition() };

		//対象との距離.
		float targetDis = D3DXVec3Length(&targetVec);

		InputRay in;
		in.mRayDir = targetVec;
		in.mRayPos = pEnemy->GetPosition();
		OutputRay out;
		if (pPlayer->HitCheckRay(in, out)) {
			for (auto&& it : pObjList) {
				InputRayPoly input;
				input.mRayPos = in.mRayPos;
				input.mRayDir = in.mRayDir;
				OutputRayPoly output;
				//レイが炎に当たっているか.
				if (it->HitCheckRayPoly(input, output)) {
					//対象との距離より短い(敵から見て手前)か.
					if (output.mDistance >= out.mDistance) {
						mIsNext = true;
						break;
					}
				}
				//当たっていなければ遷移.
				else {
					mIsNext = true;
					break;
				}
			}
		}
	}
}