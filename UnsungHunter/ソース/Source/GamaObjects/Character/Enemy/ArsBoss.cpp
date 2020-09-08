#include "ArsBoss.h"
#include "Parts/ArsAllEnemyPartsIncludes.h"
#include "../../../Scene/BaseScene/ArsScene.h"
#include "../../Weapon/Arrow/ArsAllArrowIncludes.h"
#include "../../../Core/Model/ArsModel.h"
#include "../../ArsAllGameObjectIncludes.h"
#include "../../../Misc/StateMachine/ArsAllStateMachineIncludes.h"
#include "../../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	Boss::Boss(void)
		: Enemy(eBoss)
		, mIsSpawned(false)
	{}
	Boss::~Boss(void)
	{
	}
	void Boss::Initialize(void)
	{
		Enemy::Initialize();
		mpModel = GetModelMgr().GetModel("Boss_01");

		//StateMachine.
		{
			auto pState = std::make_shared<GoToPlayerState>();
			auto GoToState = pState->Cast<GoToPlayerState>(eGoToPlayerState);
			if (GoToState) {
				GoToState->SetNextStateDistance(4.0f);
				auto pGoToState = std::make_shared <StateChanger>(GoToState);
				pGoToState->SetNextRegisterName("AttackPlayer");
				mpStateMachine->Register("GoToPlayer", pGoToState);
			}
		}

		//�n�ʂ���̍���.
		mHeight = 1.725f;

		//TODO:Json���g���Ċy���邱��!
		//HP�̐ݒ�.
		mpHitPoint = std::make_shared<Clamp<int>>(300, 0, 300);
		//�X�^�~�i�̐ݒ�.
		mpStamina = std::make_shared<Clamp<float>>(100.0f, 0.0f, 100.0f);

		auto pVirtualParent = std::make_shared<ChildGameObject>(eWolf);
		pVirtualParent->Initialize("Virtual");
		mpChild = pVirtualParent;

		auto pFrontLeftLeg = std::make_shared<FrontLeftLeg>();
		pFrontLeftLeg->Initialize("BossFrontLeftLeg_01");
		pFrontLeftLeg->SetParent(shared_from_this());
		mpChild->AddChild(pFrontLeftLeg);

		auto pFrontRightLeg = std::make_shared<FrontRightLeg>();
		pFrontRightLeg->Initialize("BossFrontRightLeg_01");
		pFrontRightLeg->SetParent(shared_from_this());
		mpChild->AddChild(pFrontRightLeg);

		auto pHindLeftLeg = std::make_shared<HindLeftLeg>();
		pHindLeftLeg->Initialize("BossHindLeftLeg_01");
		pHindLeftLeg->SetParent(shared_from_this());
		mpChild->AddChild(pHindLeftLeg);

		auto pHindRightLeg = std::make_shared<HindRightLeg>();
		pHindRightLeg->Initialize("BossHindRightLeg_01");
		pHindRightLeg->SetParent(shared_from_this());
		mpChild->AddChild(pHindRightLeg);
	}
	void Boss::Update(float aDeltaTime)
	{
		//HP����ł����.
		if (mpHitPoint->IsEmpty()) {

			mpStateMachine->Update(aDeltaTime);

			if (mIsHit) { mIsHit = false; }

			mpChild->Update(mMatrix, aDeltaTime);

			mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
			return;
		}

		//�I�I�J�~����x���������Ă��Ȃ���Ԃő̗͂����ȉ��ɂȂ�Ώ���.
		constexpr int SpawnedValue = 230;
		if (!mIsSpawned && mpHitPoint->GetCurrentValue() < SpawnedValue) {
			mIsSpawned = true;
			SpawnWolf();
		}

		Enemy::Update(aDeltaTime);

		//�����蔻��.
		OnCollision();

		//�q�̍X�V.
		mpChild->Update(mMatrix, aDeltaTime);
		//�����Ă���΃A�j���[�V����.
		if (mpStateMachine->GetFlg("Move")) {
			mpChild->WalkAnimation();
		}
	}
	void Boss::Draw3D(void)
	{
		Enemy::Draw3D();
	}
	void Boss::OnCollision(void)
	{
		//HP���O�ȉ��Ȃ瓖���蔻������Ȃ�.
		if (mpHitPoint->IsEmpty()) { return; }

		{//Wall.
			D3DXVECTOR3 front{ 0.0f,0.0f,-1.0f };

			D3DXVec3TransformNormal(&front, &front, &mRotateMatrix);

			InputRay input;
			input.mRayPos = GetPosition();
			input.mRayDir = front;
			OutputRay output;

			auto pObj = mpScene->FindGameObject(eWall);
			if (pObj) {
				auto pWall = pObj->Cast<Wall>(eWall);
				if (pWall) {
					pWall->HitCheckRay(input, output);
					AddPosition(output.mForceOut);
				}
			}
		}

		{//�v���C���[.
			SPtr<GameObject> player = mpScene->FindGameObject(ePlayer);

			if (!player) { mpStateMachine->ChangeState("RotatePlayer"); return; }

			InputSphere inputInfo;
			inputInfo.mCenterPos = GetPosition();
			inputInfo.mRadius = 1.0f;

			OutputSphere outputInfo;
			if (player->HitCheckSphere(inputInfo, outputInfo)) {
				//�U����Ԃł���΃_���[�W��^����.
				if (mpStateMachine->GetFlg("Attack")) {
					auto pPlayer = player->Cast<Character>(ePlayer);
					constexpr int JumpAttackDamage = 30;
					pPlayer->Damage(JumpAttackDamage, "EnemyJumpAttack");
				}
			}

		}

		{//FireWall.
			std::list<SPtr<GameObject>> firewalls = mpScene->FindGameObjectList(eFireWallFragment);
			InputSphere inputinfo;
			inputinfo.mCenterPos = GetPosition();
			inputinfo.mRadius = 0.5f;

			//��łȂ����.
			if (!firewalls.empty()) {
				for (auto&& it : firewalls) {
					OutputSphere outputinfo;
					if (it->HitCheckSphere(inputinfo, outputinfo)) {
						constexpr int FireWallDamage = 3;
						Damage(FireWallDamage, "FireWall");
					}
				}
			}
		}

		{//Tree.
			std::list<SPtr<GameObject>> trees = mpScene->FindGameObjectList(eTree);
			InputCircle inputinfo;
			inputinfo.mCenterPos = GetPosition();
			inputinfo.mCenterPos.y = 0.0f;
			inputinfo.mCollisionLength = 1.75f;


			for (auto&& it : trees) {
				OutputCircle outputinfo;
				if (it->HitCheckCircle(inputinfo, outputinfo)) {
					D3DXMATRIX tmpMat;
					D3DXVECTOR3 vec = outputinfo.mForceOut;
					D3DXMatrixTranslation(&tmpMat, vec.x, vec.y, vec.z);
					mTransMatrix *= tmpMat;
				}
			}
		}

		{//Wolf.
			std::list<SPtr<GameObject>> enemies = mpScene->FindGameObjectList(eWolf);
			InputCircle inputinfo;
			inputinfo.mCenterPos = GetPosition();
			inputinfo.mCollisionLength = 1.0f;


			for (auto&& it : enemies) {
				OutputCircle outputinfo;
				if (it->HitCheckCircle(inputinfo, outputinfo)) {
					D3DXMATRIX tmpMat;
					D3DXVECTOR3 vec = outputinfo.mForceOut;
					D3DXMatrixTranslation(&tmpMat, vec.x, 0.0f, vec.z);
					mTransMatrix *= tmpMat;
				}
			}
		}
	}
	void Boss::SpawnWolf(void)
	{
		auto pWolf = std::make_shared<Wolf>();
		mpScene->AddGameObject(pWolf, 4U);
		pWolf->Initialize();
		pWolf->SetPosition({ 3.0f, 0.0f, 165.0f });
		pWolf->ChangeRotateState();

		auto pWolf1 = std::make_shared<Wolf>();
		mpScene->AddGameObject(pWolf1, 4U);
		pWolf1->Initialize();
		pWolf1->SetPosition({ 2.0f, 0.0f, 104.0f });
		pWolf1->ChangeRotateState();

		auto pWolf2 = std::make_shared<Wolf>();
		mpScene->AddGameObject(pWolf2, 4U);
		pWolf2->Initialize();
		pWolf2->SetPosition({ -29.0f, 0.0f, 135.0f });
		pWolf2->ChangeRotateState();

		//for (int i = 0; i < 3; i++) {
		//	auto pWolf = std::make_shared<Wolf>();
		//	mpScene->AddGameObject(pWolf, 4U);
		//	pWolf->Initialize();
		//	pWolf->SetPosition();
		//	pWolf->ChangeRotateState();
		//}
	}
}