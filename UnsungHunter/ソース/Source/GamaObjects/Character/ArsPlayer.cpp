#include "ArsPlayer.h"
#include "../../Core/ArsIncludeCores.h"
#include "Parts/ArsAllPartsIncludes.h"
#include "../../Scene/BaseScene/ArsScene.h"
#include "../Weapon/ArsAllWeaponIncludes.h"
#include "../UserInterface/ArsUserInterface.h"
#include "../Stage/ArsWall.h"

namespace Alrescha {
	Player::Player(void)
		: Character(ePlayer)
		, mpChild(nullptr)
		, mpBow(nullptr)
		, mpQuiver(nullptr)
		, mWaitTimeManager()
		, mIsMove(false)
		, mIsBendBow(false)
		, mIsShootArrow(false)
		, mSelectedArrowType(ArrowType::eNormalArrow)
		, mpNumOfArrowsForFire(nullptr)
	{}
	Player::~Player(void)
	{
	}
	void Player::Deserialize(const json11::Json& aJsonObject)
	{
	}
	void Player::Initialize(void)
	{
		GameObject::Initialize();

		mpModel = GetModelMgr().GetModel("Body_01");

		//HP�̐ݒ�.
		constexpr int MaxHp = 100;
		constexpr int MinHp = 0;
		constexpr int CurrentHp = 100;
		mpHitPoint = std::make_shared<Clamp<int>>(MaxHp, MinHp, CurrentHp);
		//�X�^�~�i�̐ݒ�.
		constexpr float MaxStamina = 100.0f;
		constexpr float MinStamina = 0.0f;
		constexpr float CurrentStamina = 100.0f;
		mpStamina = std::make_shared<Clamp<float>>(MaxStamina, MinStamina, CurrentStamina);
		//��̐ݒ�.
		constexpr int MaxFireArrowValue = 3;
		constexpr int MinFireArrowValue = 0;
		constexpr int CurrentFireArrowValue = 3;
		mpNumOfArrowsForFire = std::make_unique<Clamp<int>>(MaxFireArrowValue, MinFireArrowValue, CurrentFireArrowValue);

		auto pVirtualParent = std::make_shared<ChildGameObject>(ePlayer);
		pVirtualParent->Initialize("Virtual");
		mpChild = pVirtualParent;

		auto pHead = std::make_shared<Head>();
		pHead->Initialize("Head_01");
		pHead->SetParent(shared_from_this());
		mpChild->AddChild(pHead);

		auto pLeftArm = std::make_shared<LeftArm>();
		pLeftArm->Initialize("LeftArm_01");
		pLeftArm->SetParent(shared_from_this());
		mpChild->AddChild(pLeftArm);

		auto pLeftLeg = std::make_shared<LeftLeg>();
		pLeftLeg->Initialize("LeftLeg_01");
		pLeftLeg->SetParent(shared_from_this());
		mpChild->AddChild(pLeftLeg);

		auto pRightLeg = std::make_shared<RightLeg>();
		pRightLeg->Initialize("RightLeg_01");
		pRightLeg->SetParent(shared_from_this());
		mpChild->AddChild(pRightLeg);

		auto pRightArm = std::make_shared<RightArm>();
		pRightArm->Initialize("RightArm_01");
		pRightArm->SetParent(shared_from_this());
		mpChild->AddChild(pRightArm);

		mpBow = std::make_shared<Bow>();
		mpBow->SetScene(mpScene);
		mpBow->Initialize();

		mpQuiver = std::make_shared<Quiver>();
		mpQuiver->SetScene(mpScene);
		mpQuiver->Initialize();

		{//WaitTime.
			//���؂�ւ�����ɁA���ɐ؂�ւ����ł���܂ł̎���.
			auto& waitTime = mWaitTimeManager.GetWaitTime("Switch");
			waitTime.SetWaitTime(0.4f);

			mWaitTimeManager.Register("MoveSound", 0.4f);
			mWaitTimeManager.Register("BendMoveSound", 0.5f);

			mInvincibleTime.Register("Switch", 0.125f);
		}
	}
	void Player::Update(float aDeltaTime)
	{
		if (mpHitPoint->IsEmpty()) { return; }

		Character::Update(aDeltaTime);

		//�X�^�~�i�Ǘ�.
		StaminaManagement();

		//�ړ�����.
		MoveAction(aDeltaTime);

		if (mIsBendBow) {
			D3DXMATRIX rotate;
			D3DXMatrixRotationY(&rotate, D3DXToRadian(90.0f));
			mRotateMatrix = rotate * mRotateMatrix;
		}

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;

		//��̑I������.
		SelectArrowAction();

		//�U������.
		AttackAction();

		//�҂����ԍX�V.
		mWaitTimeManager.Update(aDeltaTime);

		//�q�A�v���C���[�����I�u�W�F�N�g���X�V.
		{
			mpChild->Update(mMatrix, aDeltaTime);

			//�|�ɕK�v�ȍ��r�̏��.
			std::list<SPtr<ChildGameObject>> pChildList;
			mpChild->GetChild(pChildList, eLeftArm);
			if (!pChildList.empty()) {
				SPtr<ChildGameObject> pLeftArm = pChildList.front();
				mpBow->Update(pLeftArm->GetMatrixConst(), aDeltaTime);
			}

			mpQuiver->Update(mMatrix, aDeltaTime);
		}

		//�����蔻��.
		OnCollision();
	}
	void Player::Draw3D(void)
	{
		Get3D().SetWorldMatrix(&mMatrix);
		if (mpHitPoint->IsEmpty()) { return; }

		//���G��Ԃ��ǂ���.
		if (mIsInvincible) {

			if (!mInvincibleTime.GetWaitTime("Switch").IsExceed(true)) {
				mpModel->Draw();
				//�q�A�v���C���[�����I�u�W�F�N�g��`��.
				mpChild->Draw();
				mpBow->Draw3D();
				mpQuiver->Draw3D();
				return;
			}
			constexpr float alphaValue = 0.3f;
			mpModel->Draw(alphaValue);
		}
		else
		{
			mpModel->Draw();

			//�q�A�v���C���[�����I�u�W�F�N�g��`��.
			mpChild->Draw();
			mpBow->Draw3D();
			mpQuiver->Draw3D();
		}
	}
	void Player::MoveAction(float aDeltaTime)
	{
		auto camera = Get3D().GetCamera();
		auto camRot = camera.GetCamRot();

		//�A�j���[�V�������Z�b�g.
		if (!mIsMove && !mIsBendBow) {
			mpChild->AllAnimeFrameReset();
		}

		mIsMove = false;
		constexpr float MoveSpeed = 12.5f;
		D3DXVECTOR3 velocity{ 0.0f,0.0f,0.0f };


		if (GetInput().GetKey('W')) {
			if (!GetInput().GetKey('S')) {
				velocity += { 0.0f, 0.0f, 1.0f };
				mIsMove = true;
			}
		}
		if (GetInput().GetKey('S')) {
			if (!GetInput().GetKey('W')) {
				velocity += { 0.0f, 0.0f, -1.0f };
				mIsMove = true;
			}
		}
		if (GetInput().GetKey('A')) {
			if (!GetInput().GetKey('D')) {
				velocity += { -1.0f, 0.0f, 0.0f };
				mIsMove = true;
			}
		}
		if (GetInput().GetKey('D')) {
			if (!GetInput().GetKey('A')) {
				velocity += { 1.0f, 0.0f, 0.0f };
				mIsMove = true;
			}
		}

		D3DXVec3TransformCoord(&velocity, &velocity, &camRot);
		velocity.y = 0.0f;
		//�����Ă��鎞�����J�x����.
		if (mIsMove) {
			InputRay input;
			input.mRayPos = GetPosition();
			input.mRayDir = velocity;
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

		//�\���Ă��Ȃ�,����,�ړ��ʂ��I�[��0�̏ꍇ.
		if (!mIsBendBow && velocity == D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { return; }

		//�ړ����̃T�E���h.
		if (mIsMove) {
			//�|���\���Ă��邩.
			if (mIsBendBow) {
				if (mWaitTimeManager.GetWaitTime("BendMoveSound").IsExceed(true)) {
					GetSoundManager().Play("MoveSound", 4U);
				}
			}
			else {
				if (mWaitTimeManager.GetWaitTime("MoveSound").IsExceed(true)) {
					GetSoundManager().Play("MoveSound", 4U);
				}
			}
		}


		//�|���\���Ă���Ƃ��͏Ə��̕����Ɍ���.
		//�΂߈ړ��������ۂɈړ����x���㏸���Ȃ��悤�ɂ���.
		if (mIsBendBow || mIsMove &&
			(velocity.x != 0.0f || velocity.z != 0.0f)) {
			D3DXVec3Normalize(&velocity, &velocity);

			//�|���\���Ă��鎞�̑��x.
			constexpr float BendSpeed = 2.5f;
			if (mIsBendBow) { velocity *= BendSpeed; }
			else { velocity *= MoveSpeed; }

			AddPosition(velocity * aDeltaTime);

			D3DXVECTOR3 playerForward{ 0.0f,0.0f,1.0f };
			D3DXVec3TransformNormal(&playerForward, &playerForward, &mRotateMatrix);

			//��]�p�x
			D3DXVec3Normalize(&velocity, &velocity);
			D3DXVec3Normalize(&playerForward, &playerForward);

			auto centerVec = mpScene->GetCamera()->GetCenterVector();
			D3DXVec3Normalize(&centerVec, &centerVec);

			float angleRad = 0.0f;
			if (mIsBendBow) { angleRad = D3DXVec3Dot(&playerForward, &centerVec); }
			else { angleRad = D3DXVec3Dot(&playerForward, &velocity); }

			angleRad = acosf(angleRad);
			constexpr float RotationSpeed = FLT_MAX;
			if (angleRad > RotationSpeed) { angleRad = RotationSpeed; }

			if (angleRad >= 0.001f) {
				//��]����
				D3DXVECTOR3 cross;
				if (mIsBendBow) { D3DXVec3Cross(&cross, &playerForward, &centerVec); }
				else { D3DXVec3Cross(&cross, &playerForward, &velocity); }

				//�������].
				if (cross.y < 0.0f) { angleRad *= -1.0f; }

				D3DXMATRIX tmpMat;
				D3DXMatrixIdentity(&tmpMat);
				D3DXMatrixRotationY(&tmpMat, angleRad);

				mRotateMatrix *= tmpMat;
			}
		}
	}

	void Player::SelectArrowAction(void)
	{
		//�z�C�[��(�㉺�֌W�Ȃ�)�Ő؂�ւ�.
		if (GetInput().GetMouseWheel()) {
			if (mWaitTimeManager.IsRegistered("Switch")) {
				//�x�����ԂȂ�ύX�������s��Ȃ�
				if (!mWaitTimeManager.GetWaitTime("Switch").IsExceed(true)) {
					return;
				}
			}
			//�I������Ă���^�C�v�ɂ���Đ؂�ւ���.
			switch (mSelectedArrowType)
			{
			case ArrowType::eNormalArrow:
				mSelectedArrowType = ArrowType::eFireArrow;
				break;
			case ArrowType::eFireArrow:
				mSelectedArrowType = ArrowType::eNormalArrow;
				break;
			default:
				_ASSERT_EXPR(false, L"�o�^����Ă��Ȃ��^�C�v���n����܂����B");
				break;
			}
		}
	}

	bool Player::NumberOfSelectArrows(void)
	{
		if (mSelectedArrowType == ArrowType::eNormalArrow) {
			//�c���P�ȏ�.
			return true;
		}
		else if (mSelectedArrowType == ArrowType::eFireArrow) {
			if (!mpNumOfArrowsForFire->IsEmpty()) {
				//�c���P�ȏ�.
				return true;
			}
		}
		//�c��0.
		return false;
	}

	void Player::AttackAction(void)
	{
		static bool isDistanceDelay = false;
		//���ɍ\������܂ł̎���(60=��1�b).
		static int waitTime = 60 * 2;

		//�����I�ɍ\�����I�����ꍇ�͈�莞�ԍ\���邱�Ƃ��ł��Ȃ�.
		if (isDistanceDelay) {
			waitTime--;
			if (waitTime < 0) {
				waitTime = 60 * 2;
				isDistanceDelay = false;
			}
		}

		static bool isShotDelay = false;
		static float WaitTime = 60.0f * 0.5f;
		//��𔭎˂����シ���ɔ��˂ł��Ȃ��悤�ɂ���.
		if (isShotDelay) {
			WaitTime--;
			if (WaitTime < 0.0f) {
				WaitTime = 60.0f * 0.5f;
				isShotDelay = false;
			}
		}

		//�X�^�~�i��0�ȏ�ł���A���A�����I�ɏI������Ă��Ȃ����(���Ԃ��o���Ă����).
		if (GetInput().GetKey(VK_RBUTTON) && !mpStamina->IsEmpty() && !isDistanceDelay) {
			{//�J��������.
				D3DXVECTOR3 camTransPos(0.7f, 0.5f, 1.75f);
				mpScene->GetCamera()->SetTransPosition(camTransPos, 10);
				mpScene->GetCamera()->ZoomIn();
				mpScene->GetCamera()->BendSensitivity();
			}
			//�t���O�ύX.
			mIsBendBow = true;
			{//�N���X�w�A�̊g�k.
				auto object = mpScene->FindGameObject(eUserInterface);
				if (!object) { return; }
				auto userinterface = object->Cast<UserInterface>(eUserInterface);
				if (!userinterface) { return; }
				userinterface->CrossHairScaleDown();
			}

			//��̐����A���˓�.
			if (GetInput().GetKey(VK_LBUTTON)) {
				if (NumberOfSelectArrows()) { mIsShootArrow = true; }
			}
			else {
				mIsShootArrow = false;
			}

			//�O�Ɍ���������莞�Ԃ����Ă����(�A�����˂�h������).
			if (GetInput().GetKeyUp(VK_LBUTTON) && !isShotDelay) {
				if (mSelectedArrowType == ArrowType::eNormalArrow) {
					mpBow->Shot(mSelectedArrowType);
				}
				else if (mSelectedArrowType == ArrowType::eFireArrow) {
					if (NumberOfSelectArrows()) {
						mpBow->Shot(mSelectedArrowType);
						mpNumOfArrowsForFire->Remove(1);
					}
				}
				isShotDelay = true;
			}
		}
		else if (mpStamina->IsEmpty()) {
			//�X�^�~�i��0�ȉ��Ȃ�.
			//�����I�ɃJ������߂�.
			mpScene->GetCamera()->SetTransPosition({ 0.0f,1.0f,0.0f }, 10);
			mpScene->GetCamera()->ZoomOut();
			mpScene->GetCamera()->NormalSensitivity();

			//�A�j���[�V�������Z�b�g.
			mpChild->AllAnimeFrameReset();

			//�����I�Ƀt���O�ύX.
			mIsBendBow = false;
			isDistanceDelay = true;
			mIsShootArrow = false;
		}

		//�\������߂��Ƃ�.
		if (GetInput().GetKeyUp(VK_RBUTTON)) {
			mpScene->GetCamera()->SetTransPosition({ 0.0f,1.0f,0.0f }, 10);
			mpScene->GetCamera()->ZoomOut();
			mpScene->GetCamera()->NormalSensitivity();

			//�q�̃A�j���[�V���������Z�b�g.
			mpChild->AllAnimeFrameReset();

			//�t���O�ύX.
			mIsBendBow = false;
			isShotDelay = false;
			mIsShootArrow = false;
		}
	}

	void Player::StaminaPlus(void)
	{
		constexpr float AddValue = 0.15f;
		//�����Ƃ��Ƃ��Ă��Ȃ����.
		if (!mIsShootArrow) { mpStamina->Add(AddValue); }
	}

	void Player::StaminaMinus(void)
	{
		constexpr float RemoveValue = 0.2f;
		//�|���\���āA��������Ă���Ό���.
		if (mIsShootArrow) { mpStamina->Remove(RemoveValue); }
	}

	void Player::StaminaManagement(void)
	{
		//�X�^�~�i����.
		StaminaMinus();

		//�X�^�~�i��.
		StaminaPlus();
	}

	void Player::OnCollision(void)
	{
		{//Terrain.
			const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
			auto info = mpScene->RayTest(GetPosition(), UnderDirection, eTerrain);
			float height = 0.9f;
			if (info.mDistance < height || info.mDistance > height) {
				D3DXMATRIX diffMat;
				D3DXMatrixTranslation(&diffMat, 0.0f, (height - info.mDistance), 0.0f);
				mTransMatrix *= diffMat;
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
						constexpr int FireDamage = 3;
						Damage(FireDamage, "FireWall");
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
			std::list<SPtr<GameObject>> enemies = mpScene->FindGameObjectList(eEnemyBegin, eEnemyEnd);

			InputCircle inputInfo;
			inputInfo.mCenterPos = GetPosition();
			inputInfo.mCollisionLength = 0.5f;

			for (auto&& it : enemies) {
				OutputCircle outputinfo;
				if (it->HitCheckCircle(inputInfo, outputinfo)) {
					D3DXMATRIX tmpMat;
					D3DXVECTOR3 vec = outputinfo.mForceOut;
					//�n�ʂɖ��܂邽�ߍ���(y)�ɂ͉e���Ȃ�
					D3DXMatrixTranslation(&tmpMat, vec.x, 0.0f, vec.z);
					mTransMatrix *= tmpMat;
				}
			}
		}
	}
}
