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
		//�U�����󂯂Ă���΋����I��KnockBackState�ɑJ��.
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		//HP��0�ȉ��Ȃ�DeathActionState�ɑJ��.
		if (pEnemy->GetHp()->IsEmpty()) { mpStateMachine->ChangeState("DeathAction"); }

		//�ړ����Ă��Ȃ����Ƃ�`����.
		mpStateMachine->SetFlg("Move", false);

		//���ʂɉ������邩�ǂ���.
		IsThereFireWallInFront();
	}

	void StopMoveState::OnChangeEvent(void)
	{
		mIsNext = false;
	}
	void StopMoveState::IsThereFireWallInFront(void)
	{
		std::list<SPtr<GameObject>> pObjList = mpScene->FindGameObjectList(eFireWallFragment);

		//�󂩂ǂ���.
		if (pObjList.empty()) {
			//�����Ȃ���ΑJ��.
			mIsNext = true;
		}

		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }

		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return; }

		D3DXVECTOR3 targetVec{ pPlayer->GetPosition() - pEnemy->GetPosition() };

		//�ΏۂƂ̋���.
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
				//���C�����ɓ������Ă��邩.
				if (it->HitCheckRayPoly(input, output)) {
					//�ΏۂƂ̋������Z��(�G���猩�Ď�O)��.
					if (output.mDistance >= out.mDistance) {
						mIsNext = true;
						break;
					}
				}
				//�������Ă��Ȃ���ΑJ��.
				else {
					mIsNext = true;
					break;
				}
			}
		}
	}
}