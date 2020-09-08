#include "ArsBaseArrow.h"
#include "../../../Scene/BaseScene/ArsScene.h"
#include "../../ArsAllGameObjectIncludes.h"
#include "../../../Core/Model/ArsModelManager.h"
#include "../../../Core/Texture/ArsTextureManager.h"
#include "../../../Core/Sound/ArsSoundManager.h"
#include "../../../Core/DirectX/ArsDirectGraphics.h"
#include "../../Effect/ArsEffect.h"

namespace Alrescha {

	BaseArrow::BaseArrow(int aType)
		: Weapon(aType)
		, RayPosition({ 0.0f,0.0f,0.0f })
		, mpHitObject()
		, mLocalPosition({ 0.0f,0.0f,0.0f })
		, mLocalRotate()
		, mShootPower(0.0f)
		, mWaitTimeManager()
		, mpEffect(nullptr)
	{}

	BaseArrow::~BaseArrow(void)
	{}

	void BaseArrow::Initialize(const D3DXVECTOR3& aShootAngle, float aPower, const D3DXVECTOR3& aShotPosition)
	{
		D3DXMatrixIdentity(&mLocalRotate);
		Weapon::Initialize();

		mpModel = GetModelMgr().GetModel("Arrow_01");

		//���ˈʒu.
		D3DXMatrixTranslation(&mTransMatrix, aShotPosition.x, aShotPosition.y, aShotPosition.z);


		//���˂���p�x.
		{
			const auto CurrentPosition = GetPosition();

			//�������������̍��W.
			const auto ShootPosition = CurrentPosition + aShootAngle;

			//�|�W�V�������������������́A���[���h���猩���e���̃x�N�g��
			D3DXVECTOR3 localX, localY, localZ;
			//Z���̕���(�������猩������̌���)
			D3DXVec3Normalize(&localZ, &(ShootPosition - CurrentPosition));
			//X���̕���(Z���ƃ��[���hY���ɐ����Ȏ�)
			D3DXVec3Cross(&localX, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &localZ);
			D3DXVec3Normalize(&localX, &localX);
			//Y���̕���(X����Z���ɐ����Ȏ�)
			D3DXVec3Cross(&localY, &localZ, &localX);
			D3DXVec3Normalize(&localY, &localY);

			D3DXMATRIX m;
			D3DXMatrixIdentity(&m);
			m._11 = localX.x; m._12 = localX.y; m._13 = localX.z;
			m._21 = localY.x; m._22 = localY.y; m._23 = localY.z;
			m._31 = localZ.x; m._32 = localZ.y; m._33 = localZ.z;

			mRotateMatrix = m;
		}
		//���˂��鑬�x.
		mShootPower = aPower;

		{//WaitTime.
			auto& waitTime = mWaitTimeManager.GetWaitTime("Delay");
			waitTime.SetWaitTime(0.00125f);
		}

		//�G�t�F�N�g�̐���.
		mpEffect = std::make_unique<Effect>(eFireArrow);
		mpEffect->Initialize();

		//���ˎ��Ɉ�x��������炷.
		GetSoundManager().Play("ShotSound");
	}

	void BaseArrow::Update(float aDeltaTime)
	{
		if (mIsHit) {
			//�擪���珙�X�ɍ폜.
			mpEffect->RemoveLogList();

			//�����Ď��Ώۂ�����ł�����������.
			if (mpHitObject.expired()) { Kill(); return; }
			//�ǔ�.
			TrackingTarget();
		}
		else {
			MoveAction(aDeltaTime);
		}
		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;

		OnCollision();

		mWaitTimeManager.Update(aDeltaTime);
	}

	void BaseArrow::Draw3D(void)
	{
		Get3D().SetWorldMatrix(&mMatrix);
		mpModel->Draw();

		mpEffect->Draw3D();
	}
	void BaseArrow::TrackingTarget(void)
	{
		auto pHitEntity = mpHitObject.lock();
		if (!pHitEntity) { return; }
		//�����������肪�L�����N�^�[�Ŗ����ꍇ�͕K�v�Ȃ�.
		if (!IsCharacter(pHitEntity->GetType())) { return; }
		//��].
		const auto RotMat = mLocalRotate * pHitEntity->GetRotateMatrix();
		mRotateMatrix = RotMat;

		//���W.
		const auto LocalPos = *D3DXVec3TransformNormal(&D3DXVECTOR3(), &mLocalPosition, &mRotateMatrix);
		const auto Pos = LocalPos + pHitEntity->GetPosition();

		D3DXMatrixTranslation(&mTransMatrix, Pos.x, Pos.y, Pos.z);
	}
	void BaseArrow::MoveAction(float aDeltaTime)
	{
		//�i�s����.
		D3DXVECTOR3 vel{ 0.0f,0.0f,1.0f };
		D3DXVec3TransformNormal(&vel, &vel, &mRotateMatrix);
		vel *= mShootPower;

		D3DXMATRIX tmpMat;
		D3DXMatrixTranslation(&tmpMat, vel.x, vel.y, vel.z);
		mTransMatrix = tmpMat * mTransMatrix;

		if (mWaitTimeManager.IsRegistered("Delay")) {
			//�x�����ԂȂ烍�O�������s��Ȃ�
			if (!mWaitTimeManager.GetWaitTime("Delay").IsExceed()) {
				return;
			}
		}

		//�I�u�W�F�N�g�ɓ������Ă���΃G�t�F�N�g�X�V�s�v.
		if (mIsHit) { return; }

		mpEffect->UpdateTransform(aDeltaTime, mMatrix);
	}
	void BaseArrow::OnCollision(void)
	{
		//��ǂ����ɓ������Ă���΃��C���肷��K�v������.
		if (mIsHit) { return; }

		//���C�̔��˕����x�N�g��.
		D3DXVECTOR3 rayDir(0.0f, 0.0f, 1.0f);
		D3DXVec3TransformNormal(&rayDir, &rayDir, &mRotateMatrix);
		//�����������b�V���Ƃ̋���.
		float distance = 0.0f;

		//���C�̔��ˈʒu���H�Ɉړ�.
		//��肩���΂��Ɗђʂ��Ă��܂����ߔ��ˈʒu���H�ɂ���.
		D3DXVECTOR3 arrowFeathers(0.0f, 0.0f, -0.45f);
		D3DXVec3TransformNormal(&arrowFeathers, &arrowFeathers, &mRotateMatrix);
		//��H������܂ł̋���.
		constexpr float ArrowheadDistance = 1.0f;
		RayPosition = GetPosition() + arrowFeathers;

		//���C����ŕK�v�ȏ����i�[.
		InputRay in;
		in.mRayDir = rayDir;
		in.mRayPos = RayPosition;
		OutputRay out;

		{//Wolf.
			std::list<SPtr<GameObject>> enemies = mpScene->FindGameObjectList(eWolf);

			for (auto&& it : enemies) {
				if (it->HitCheckRay(in, out)) {
					if (out.mDistance <= ArrowheadDistance) {
						mIsHit = true;
						//�������������ێ�.
						mpHitObject = it->shared_from_this();
						auto pEnemy = it->Cast<Enemy>(eWolf);
						if (!pEnemy) { continue; }

						//�m�b�N�o�b�N������x�N�g���̐���.
						rayDir.y = 0;
						D3DXVec3Normalize(&rayDir, &rayDir);
						//������΂�����.
						constexpr float ImpactY = 0.5f;
						rayDir.y = ImpactY;
						//������΂���.
						rayDir *= 0.25f;
						pEnemy->SetImpactDirection(rayDir);

						//���̐�[�Ɉړ�(�␳)����.
						D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
						D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
						//rayDir:�m�b�N�o�b�N�ňړ�������(Y��������)���v���X.
						D3DXVECTOR3 knockVec = { rayDir.x,0.0f,rayDir.z };
						const auto MyPos = GetPosition() + shift + knockVec;
						D3DXMatrixTranslation(&mTransMatrix, MyPos.x, MyPos.y, MyPos.z);

						//Wolf�̃G���e�B�e�B�̍��W���猩�����ΓI�Ȉʒu.
						mLocalPosition = MyPos - it->GetPosition();
						D3DXMATRIX invMat;
						D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
						D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);


						//�t�s��.
						D3DXMatrixInverse(&mLocalRotate, nullptr, &it->GetRotateMatrix());
						//�E���t�̉�]�s��������Ă��΁A��h�������p�x�ɂȂ�
						//�E���t����]����Ζ����]����.
						mLocalRotate *= mRotateMatrix;

						//�_���[�W�ʂƍU������`����.
						pEnemy->Damage(mDamage, "ArrowDamage");
						pEnemy->Hit();
					}
				}
			}
		}

		{//Tree.
			std::list<SPtr<GameObject>> trees = mpScene->FindGameObjectList(eTree);

			for (auto&& it : trees) {
				if (it->HitCheckRay(in, out)) {
					if (out.mDistance <= ArrowheadDistance) {
						mIsHit = true;
						//�������������ێ�.
						mpHitObject = it->shared_from_this();

						//���̐�[�Ɉړ�(�␳)����.
						D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
						D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
						const auto MyPos = GetPosition() + shift;
						D3DXMatrixTranslation(&mTransMatrix, MyPos.x, MyPos.y, MyPos.z);

						mLocalPosition = MyPos - it->GetPosition();
						D3DXMATRIX invMat;
						D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
						D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);

						//�t�s��.
						D3DXMatrixInverse(&mLocalRotate, nullptr, &it->GetRotateMatrix());

						mLocalRotate *= mRotateMatrix;

					}
				}
			}
		}

		{//SkyDome.
			SPtr<GameObject> skydome = mpScene->FindGameObject(eSkyDome);

			if (skydome->HitCheckRay(in, out)) {
				if (out.mDistance <= ArrowheadDistance) {
					Kill();
				}
			}
		}

		{//Terrain.
			SPtr<GameObject> terrain = mpScene->FindGameObject(eTerrain);

			if (terrain->HitCheckRay(in, out)) {
				if (out.mDistance <= ArrowheadDistance) {
					mIsHit = true;
					//�������������ێ�.
					mpHitObject = terrain->shared_from_this();

					//���̐�[�Ɉړ�(�␳)����.
					D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
					D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
					const auto MyPos = GetPosition() + shift;

					//Terrain�̃G���e�B�e�B�̍��W���猩�����ΓI�Ȉʒu.
					mLocalPosition = MyPos - terrain->GetPosition();
					D3DXMATRIX invMat;
					D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
					D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);

					//�t�s��.
					D3DXMatrixInverse(&mLocalRotate, nullptr, &terrain->GetRotateMatrix());
					mLocalRotate *= mRotateMatrix;
				}
			}
		}

		{//Boss.
			SPtr<GameObject> objects = mpScene->FindGameObject(eBoss);
			if (!objects) { return; }
			SPtr<Enemy> boss = objects->Cast<Enemy>(eBoss);
			if (!boss) { return; }

			//HP���O�ȉ��Ȃ瓖���蔻������Ȃ�.
			//���̏������Ȃ��Ɠ����ɂȂ���GameObject���폜�����܂ł̊Ԗ�˂��h�����Ă��܂�.
			if (boss->GetHp()->IsEmpty()) { return; }

			if (boss->HitCheckRay(in, out)) {
				if (out.mDistance <= ArrowheadDistance) {
					mIsHit = true;
					//�������������ێ�.
					mpHitObject = boss->shared_from_this();

					//�m�b�N�o�b�N������x�N�g���̐���.
					rayDir.y = 0;
					D3DXVec3Normalize(&rayDir, &rayDir);
					//������΂�����.
					constexpr float ImpactY = 0.5f;
					rayDir.y = ImpactY;
					//������΂���.
					rayDir *= 0.25f;
					boss->SetImpactDirection(rayDir);

					//���̐�[�Ɉړ�(�␳)����.
					D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
					D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
					//rayDir:�m�b�N�o�b�N�ňړ����������v���X.
					D3DXVECTOR3 knockVec = { rayDir.x,rayDir.y,rayDir.z };
					const auto MyPos = GetPosition() + shift + knockVec;
					D3DXMatrixTranslation(&mTransMatrix, MyPos.x, MyPos.y, MyPos.z);

					//Boss�̃G���e�B�e�B�̍��W���猩�����ΓI�Ȉʒu.
					mLocalPosition = MyPos - boss->GetPosition();
					D3DXMATRIX invMat;
					D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
					D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);


					//�t�s��.
					D3DXMatrixInverse(&mLocalRotate, nullptr, &boss->GetRotateMatrix());
					//�E���t�̉�]�s��������Ă��΁A��h�������p�x�ɂȂ�
					//�E���t����]����Ζ����]����.
					mLocalRotate *= mRotateMatrix;

					//�_���[�W�ʂƍU������`����.
					boss->Damage(mDamage, "ArrowDamage");
					boss->Hit();
				}
			}
		}
	}
}