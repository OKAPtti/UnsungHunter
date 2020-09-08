#include "ArsWolf.h"
#include "Parts/ArsAllEnemyPartsIncludes.h"
#include "../../../Scene/BaseScene/ArsScene.h"
#include "../../Weapon/Arrow/ArsAllArrowIncludes.h"
#include "../../../Core/Model/ArsModel.h"
#include "../../ArsAllGameObjectIncludes.h"
#include "../../../Misc/StateMachine/ArsAllStateMachineIncludes.h"
#include "../../../Core/Input/ArsInput.h"
#include "../../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	Wolf::Wolf(void)
		: Enemy(eWolf)
	{}

	Wolf::~Wolf(void)
	{}
	void Wolf::Initialize(void)
	{
		Enemy::Initialize();
		mpModel = GetModelMgr().GetModel("Wolf_01");

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

		//TODO:JsonÇégÇ¡ÇƒäyÇ∑ÇÈÇ±Ç∆!
		//HPÇÃê›íË.
		mpHitPoint = std::make_shared<Clamp<int>>(100, 0, 100);
		//ÉXÉ^É~ÉiÇÃê›íË.
		mpStamina = std::make_shared<Clamp<float>>(100.0f, 0.0f, 100.0f);

		auto pVirtualParent = std::make_shared<ChildGameObject>(eWolf);
		pVirtualParent->Initialize("Virtual");
		mpChild = pVirtualParent;

		auto pFrontLeftLeg = std::make_shared<FrontLeftLeg>();
		pFrontLeftLeg->Initialize("FrontLeftLeg_01");
		pFrontLeftLeg->SetParent(shared_from_this());
		mpChild->AddChild(pFrontLeftLeg);

		auto pFrontRightLeg = std::make_shared<FrontRightLeg>();
		pFrontRightLeg->Initialize("FrontRightLeg_01");
		pFrontRightLeg->SetParent(shared_from_this());
		mpChild->AddChild(pFrontRightLeg);

		auto pHindLeftLeg = std::make_shared<HindLeftLeg>();
		pHindLeftLeg->Initialize("HindLeftLeg_01");
		pHindLeftLeg->SetParent(shared_from_this());
		mpChild->AddChild(pHindLeftLeg);

		auto pHindRightLeg = std::make_shared<HindRightLeg>();
		pHindRightLeg->Initialize("HindRightLeg_01");
		pHindRightLeg->SetParent(shared_from_this());
		mpChild->AddChild(pHindRightLeg);
	}
	void Wolf::Update(float aDeltaTime)
	{
		if (mpHitPoint->IsEmpty()) {

			mpStateMachine->Update(aDeltaTime);

			if (mIsHit) { mIsHit = false; }

			mpChild->Update(mMatrix, aDeltaTime);

			mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
			return;
		}

		Enemy::Update(aDeltaTime);

		OnCollision();

		mpChild->Update(mMatrix, aDeltaTime);
		if (mpStateMachine->GetFlg("Move")) {
			mpChild->WalkAnimation();
		}
	}
	void Wolf::Draw3D(void)
	{
		Enemy::Draw3D();
	}
	void Wolf::OnCollision(void)
	{
		//HPÇ™ÇOà»â∫Ç»ÇÁìñÇΩÇËîªíËÇÇµÇ»Ç¢.
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

		{//Player.
			//É{ÉXÇ™Ç¢Ç»ÇØÇÍÇŒçUåÇÇÕó^Ç¶Ç»Ç¢.
			auto pBoss = mpScene->FindGameObject(eBoss);
			if (!pBoss) { return; }

			SPtr<GameObject> player = mpScene->FindGameObject(ePlayer);

			if (player) {

				InputSphere inputInfo;
				inputInfo.mCenterPos = GetPosition();
				inputInfo.mRadius = 0.5f;

				OutputSphere outputInfo;
				if (player->HitCheckSphere(inputInfo, outputInfo)) {
					//çUåÇèÛë‘Ç≈Ç†ÇÍÇŒÉ_ÉÅÅ[ÉWÇó^Ç¶ÇÈ.
					if (mpStateMachine->GetFlg("Attack")) {
						auto pPlayer = player->Cast<Character>(ePlayer);
						constexpr int JumpAttackDamage = 10;
						pPlayer->Damage(JumpAttackDamage, "EnemyJumpAttack");
					}
				}
			}
		}

		{//FireWall.
			std::list<SPtr<GameObject>> firewalls = mpScene->FindGameObjectList(eFireWallFragment);
			InputSphere inputinfo;
			inputinfo.mCenterPos = GetPosition();
			inputinfo.mRadius = 0.5f;

			SPtr<GameObject> player = mpScene->FindGameObject(ePlayer);


			D3DXVECTOR3 targetVec{ player->GetPosition() - GetPosition() };
			D3DXVec3Normalize(&targetVec, &targetVec);
			targetVec.y = 0.0f;

			InputRayPoly in;
			in.mRayDir = *D3DXVec3TransformNormal(&D3DXVECTOR3(), &targetVec, &mRotateMatrix);
			in.mRayPos = GetPosition();

			//ãÛÇ≈Ç»ÇØÇÍÇŒ.
			if (!firewalls.empty()) {
				for (auto&& it : firewalls) {
					OutputSphere outputinfo;
					OutputRayPoly out;
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



			for (auto&& it : enemies) {
				OutputCircle outputinfo;
				inputinfo.mCollisionLength = 1.5f;
				//è’ìÀ.
				if (it->HitCheckCircle(inputinfo, outputinfo)) {
					D3DXMATRIX tmpMat;
					D3DXVECTOR3 vec = outputinfo.mForceOut;
					D3DXMatrixTranslation(&tmpMat, vec.x, 0.0f, vec.z);
					mTransMatrix *= tmpMat;
				}
				//îÕàÕì‡Ç…Ç¢ÇÈWolfÇÃàÍïCÇ™çUåÇÇéÛÇØÇΩèÍçáStateÇRotatePlayerÇ…ïœçX.
				inputinfo.mCollisionLength = 100.0f;
				if (it->HitCheckCircle(inputinfo, outputinfo)) {
					auto pWolf = it->Cast<Wolf>(eWolf);
					if (pWolf->GetIsHit()) {
						ChangeRotateState();
						continue;
					}
				}
			}
		}
	}
	void Wolf::ChangeRotateState(void)
	{
		mpStateMachine->ChangeState("RotatePlayer");
	}
}