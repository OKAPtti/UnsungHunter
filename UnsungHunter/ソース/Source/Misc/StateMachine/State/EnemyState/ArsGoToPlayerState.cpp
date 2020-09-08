#include "ArsGoToPlayerState.h"
#include "../../../../Core/Model/ArsModel.h"
#include "../../../../Scene/BaseScene/ArsScene.h"
#include "../../../../GamaObjects/Character/ArsAllCharactersIncludes.h"

namespace Alrescha {
	GoToPlayerState::GoToPlayerState(void)
		: EnemyState(eGoToPlayerState)
		, mNextStateLength(0.0f)
		, mPlayerPos({ 0.0f,0.0f,0.0f })
	{}
	GoToPlayerState::~GoToPlayerState(void)
	{}
	void GoToPlayerState::Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)
	{
		EnemyState::Start(apStateMachine, apGameObject, apScene);

	}
	void GoToPlayerState::Update(float aDeltaTime)
	{
		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return; }
		//�U�����󂯂Ă���΋����I��KnockBackState�ɑJ��.
		if (pEnemy->GetIsHit()) { mpStateMachine->ChangeState("KnockBack"); pEnemy->NotIsHit(); }

		//���̕ǂ����邩�ǂ���.
		if (IsThereFireWallInFront()) { mpStateMachine->ChangeState("StopMove"); return; }

		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return; }

		//HP�͂O�ȉ��Ȃ�DeathActionState�ɑJ��.
		if (pEnemy->GetHp()->IsEmpty()) {
			mpStateMachine->ChangeState("DeathAction");
			return;
		}

		//��x�����v���C���[�̍��W���擾.
		mPlayerPos = pPlayer->GetPosition();

		const auto EnemyMatrix = pEnemy->GetRotateMatrix();
		const auto PlayerPos = pPlayer->GetPosition();

		//�G�̌���
		D3DXVECTOR3 forward;
		D3DXVec3TransformNormal(&forward, &D3DXVECTOR3{ 0.0f,0.0f,1.0f }, &EnemyMatrix);
		D3DXVec3Normalize(&forward, &forward);

		//��x�N�g������̊p�x
		float baseAngle = 0;
		baseAngle = D3DXVec3Dot(&forward, &D3DXVECTOR3{ 0.0f,0.0f,1.0f });
		baseAngle = D3DXToDegree(acos(baseAngle));
		if (forward.x < 0.0f) { baseAngle *= -1.0f; }

		//�v���C���[�ւ̊p�x
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
			//�z����h��.
			if (angle > -RotSpeed) {
				rotY = angle;
			}
			else { rotY = -RotSpeed; }
		}
		else if (angle > 0) {
			//�z����h��.
			if (angle < RotSpeed) {
				rotY = angle;
			}
			else { rotY = RotSpeed; }
		}

		D3DXMATRIX rotate;
		D3DXMatrixRotationY(&rotate, D3DXToRadian(rotY));
		pEnemy->AddRotation(rotate);

		//�v���C���[�̕����ֈړ�������.
		D3DXVECTOR3 TargetVec = mPlayerPos - pEnemy->GetPosition();
		D3DXVec3Normalize(&TargetVec, &TargetVec);

		constexpr float MoveSpeed = 0.175f;
		TargetVec *= MoveSpeed;
		//�������蒾�񂾂肵�Ȃ��悤��y��0.0f��.
		TargetVec.y = 0.0f;

		//�؂�΂���~��Ă��鎞�ɒn�ʂɍ~���悤�ɂ���.
		const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
		auto info = mpScene->RayTest(pEnemy->GetPosition(), UnderDirection, eTerrain);
		if (info.mDistance >= pEnemy->GetHeight()) {
			TargetVec.y = pEnemy->GetHeight() - info.mDistance;
		}

		auto pObj = mpScene->FindGameObject(eBoss);
		if (pObj) {
			auto pBoss = pObj->Cast<Boss>(eBoss);
			if (pBoss->GetHp()->IsEmpty() || !pBoss->GetIsAlive()) {
				mpStateMachine->ChangeState("RotatePlayer");
				return;
			}
		}

		pEnemy->AddPosition(TargetVec);

		//�ړ��������Ƃ�`����.
		mpStateMachine->SetFlg("Move", true);

		//�w�肵�������ȓ��ł���Ύ��̏�ԂɑJ��.
		D3DXVECTOR3 vec = pEnemy->GetPosition() - mPlayerPos;
		float length = D3DXVec3Length(&vec);
		if (length < mNextStateLength) {
			mIsNext = true;
		}
	}
	void GoToPlayerState::OnChangeEvent(void)
	{
		mIsNext = false;
		mpStateMachine->SetFlg("Move", false);
		mIsForciblyChange = false;
	}
	bool GoToPlayerState::IsThereFireWallInFront(void)
	{
		std::list<SPtr<GameObject>> pObjList = mpScene->FindGameObjectList(eFireWallFragment);

		//�󂩂ǂ���.
		if (pObjList.empty()) {
			//�����Ȃ���΂���K�v���Ȃ�.
			return false;
		}

		auto pEnemy = mpEnemy.lock();
		if (!pEnemy) { return false; }

		SPtr<GameObject> pFindPlayer = mpScene->FindGameObject(ePlayer);
		if (!pFindPlayer) { return false; }
		SPtr<Character> pPlayer = pFindPlayer->Cast<Character>(ePlayer);
		if (!pPlayer) { return false; }

		//�v���C���[�����ւ̃x�N�g��.
		D3DXVECTOR3 targetVec{ pPlayer->GetPosition() - pEnemy->GetPosition() };

		//�G�ƃv���C���[�̋���.
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
				if (it->HitCheckRayPoly(input, output)) {
					//�v���C���[�Ƃ̋������Z��(�G���猩�Ď�O)��.
					if (output.mDistance <= out.mDistance) {
						return true;
					}
				}
			}
		}
		//�v���C���[�Ƃ̊Ԃɉ����Ȃ�����.
		return false;
	}

}
