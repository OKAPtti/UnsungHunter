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
		//UŒ‚‚ğó‚¯‚Ä‚¢‚ê‚Î‹­§“I‚ÉKnockBackState‚É‘JˆÚ.
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		//HP‚Í0ˆÈ‰º‚È‚çDeathActionState‚É‘JˆÚ.
		if (pEnemy->GetHp()->IsEmpty()) { mpStateMachine->ChangeState("DeathAction"); }

		//ˆÚ“®‚µ‚Ä‚¢‚È‚¢‚±‚Æ‚ğ“`‚¦‚é.
		mpStateMachine->SetFlg("Move", false);

		//³–Ê‚É‰Š‚ª‚ ‚é‚©‚Ç‚¤‚©.
		IsThereFireWallInFront();
	}

	void StopMoveState::OnChangeEvent(void)
	{
		mIsNext = false;
	}
	void StopMoveState::IsThereFireWallInFront(void)
	{
		std::list<SPtr<GameObject>> pObjList = mpScene->FindGameObjectList(eFireWallFragment);

		//‹ó‚©‚Ç‚¤‚©.
		if (pObjList.empty()) {
			//‰Š‚ª‚È‚¯‚ê‚Î‘JˆÚ.
			mIsNext = true;
		}

		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }

		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return; }

		D3DXVECTOR3 targetVec{ pPlayer->GetPosition() - pEnemy->GetPosition() };

		//‘ÎÛ‚Æ‚Ì‹——£.
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
				//ƒŒƒC‚ª‰Š‚É“–‚½‚Á‚Ä‚¢‚é‚©.
				if (it->HitCheckRayPoly(input, output)) {
					//‘ÎÛ‚Æ‚Ì‹——£‚æ‚è’Z‚¢(“G‚©‚çŒ©‚Äè‘O)‚©.
					if (output.mDistance >= out.mDistance) {
						mIsNext = true;
						break;
					}
				}
				//“–‚½‚Á‚Ä‚¢‚È‚¯‚ê‚Î‘JˆÚ.
				else {
					mIsNext = true;
					break;
				}
			}
		}
	}
}