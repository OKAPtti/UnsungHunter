#include "ArsEnemy.h"
#include "Parts/ArsAllEnemyPartsIncludes.h"
#include "../../../Scene/BaseScene/ArsScene.h"
#include "../../../Misc/StateMachine/ArsStateMachine.h"
#include "../../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	Enemy::Enemy(int aType)
		: Character(aType)
		, mpStateMachine(nullptr)
		, mHeight(0.625f)
		, mAlphaValue(1.0f)
		, mHitPosition({ 0.0f,0.0f,0.0f })
		, mReceivedImpactDirection({ 0.0f,0.0f,0.0f })
	{}
	Enemy::~Enemy(void)
	{}
	void Enemy::Initialize(void)
	{
		{//StateMachine.
			mpStateMachine = std::make_unique<StateMachine>();
			mpStateMachine->SetUp(shared_from_this(), mpScene);

			auto pFreeState = std::make_shared<StateChanger>(std::make_shared<FreeMoveState>());
			pFreeState->SetNextRegisterName("LookPlayer");
			mpStateMachine->Register("FreeMove", pFreeState);

			auto pLookState = std::make_shared<StateChanger>(std::make_shared<LookPlayerState>());
			pLookState->SetNextRegisterName("RotatePlayer");
			mpStateMachine->Register("LookPlayer", pLookState);

			auto pAttackState = std::make_shared<StateChanger>(std::make_shared<AttackPlayerState>());
			pAttackState->SetNextRegisterName("RotatePlayer");
			mpStateMachine->Register("AttackPlayer", pAttackState);

			auto pRotateState = std::make_shared<StateChanger>(std::make_shared<RotatePlayer>());
			pRotateState->SetNextRegisterName("GoToPlayer");
			mpStateMachine->Register("RotatePlayer", pRotateState);

			auto pKnockBackState = std::make_shared<StateChanger>(std::make_shared<KnockBackState>());
			pKnockBackState->SetNextRegisterName("RotatePlayer");
			mpStateMachine->Register("KnockBack", pKnockBackState);

			auto pStopMoveState = std::make_shared<StateChanger>(std::make_shared<StopMoveState>());
			pStopMoveState->SetNextRegisterName("RotatePlayer");
			mpStateMachine->Register("StopMove", pStopMoveState);

			auto pDeathActionState = std::make_shared<StateChanger>(std::make_shared<DeathActionState>());
			pDeathActionState->SetNextRegisterName("DeathAction");
			mpStateMachine->Register("DeathAction", pDeathActionState);

			//�Q�[���J�n����State���Z�b�g����.
			mpStateMachine->SetStartState("FreeMove");
		}
		Character::Initialize();
	}
	void Enemy::Update(float aDeltaTime)
	{
		Character::Update(aDeltaTime);

		mpStateMachine->Update(aDeltaTime);

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;

		{//�����蔻��.
			const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
			auto info = mpScene->RayTest(GetPosition(), UnderDirection, eTerrain);
			if (info.mDistance <= mHeight) {
				D3DXMATRIX diffMat;
				D3DXMatrixTranslation(&diffMat, 0.0f, (mHeight - info.mDistance), 0.0f);
				mTransMatrix *= diffMat;
			}

			std::list<SPtr<GameObject>> firewalls = mpScene->FindGameObjectList(eFragmentBegin, eFragmentEnd);
			InputSphere in;
			in.mCenterPos = GetPosition();
			in.mRadius = 1.0f;
			for (auto&& it : firewalls) {
				OutputSphere out;
				if (it->HitCheckSphere(in, out)) {
					constexpr int FireWallDamage = 3;
					Damage(FireWallDamage, "FireWall");
					mpStateMachine->SetFlg("FireDamage", true);
				}
			}
		}
	}
	void Enemy::Draw3D(void)
	{
		auto pDevice = Get3D().GetDevice();

		pDevice->SetTransform(D3DTS_WORLD, &mMatrix);

		//����������.
		if (mpHitPoint->IsEmpty()) {

			LPD3DXMESH pMesh = mpModel->GetMesh();
			auto materialList = mpModel->GetMaterialList();
			auto textureList = mpModel->GetTextureList();

			D3DMATERIAL9 material;
			for (size_t i = 0; i < materialList.size(); i++) {
				material = materialList[i];
				material.Diffuse.a = mAlphaValue;
				pDevice->SetMaterial(&material);
				pDevice->SetTexture(0, textureList[i]);
				pMesh->DrawSubset(i);
			}

			constexpr float RemoveAlphaValue = -0.005f;
			mAlphaValue += RemoveAlphaValue;
			//�A���t�@�l�����ȉ��ɂȂ�΍폜.
			if (mAlphaValue < 0.0f) {
				mAlphaValue = 0.0f;
				Kill();
			}
			//�q�̍X�V.
			mpChild->Draw(mAlphaValue);
		}

		//�m�b�N�o�b�N��Ԃ܂��́A���̃_���[�W��H����Ă��邽�ߐԐF�ɕ\������.
		if (mpStateMachine->GetFlg("KnockBack") || mpStateMachine->GetFlg("FireDamage")) {

			mpModel->DrawRed();

			mpStateMachine->SetFlg("FireDamage", false);
			mpChild->DrawRed();

		}
		else if (!mpHitPoint->IsEmpty())
		{
			mpModel->Draw();
			//�q�̍X�V.
			mpChild->Draw(mAlphaValue);
		}
	}
}
