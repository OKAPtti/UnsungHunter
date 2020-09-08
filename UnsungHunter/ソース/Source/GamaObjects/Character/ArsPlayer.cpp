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

		//HPの設定.
		constexpr int MaxHp = 100;
		constexpr int MinHp = 0;
		constexpr int CurrentHp = 100;
		mpHitPoint = std::make_shared<Clamp<int>>(MaxHp, MinHp, CurrentHp);
		//スタミナの設定.
		constexpr float MaxStamina = 100.0f;
		constexpr float MinStamina = 0.0f;
		constexpr float CurrentStamina = 100.0f;
		mpStamina = std::make_shared<Clamp<float>>(MaxStamina, MinStamina, CurrentStamina);
		//矢の設定.
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
			//矢を切り替えた後に、次に切り替えができるまでの時間.
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

		//スタミナ管理.
		StaminaManagement();

		//移動処理.
		MoveAction(aDeltaTime);

		if (mIsBendBow) {
			D3DXMATRIX rotate;
			D3DXMatrixRotationY(&rotate, D3DXToRadian(90.0f));
			mRotateMatrix = rotate * mRotateMatrix;
		}

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;

		//矢の選択処理.
		SelectArrowAction();

		//攻撃処理.
		AttackAction();

		//待ち時間更新.
		mWaitTimeManager.Update(aDeltaTime);

		//子、プレイヤーが持つオブジェクトを更新.
		{
			mpChild->Update(mMatrix, aDeltaTime);

			//弓に必要な左腕の情報.
			std::list<SPtr<ChildGameObject>> pChildList;
			mpChild->GetChild(pChildList, eLeftArm);
			if (!pChildList.empty()) {
				SPtr<ChildGameObject> pLeftArm = pChildList.front();
				mpBow->Update(pLeftArm->GetMatrixConst(), aDeltaTime);
			}

			mpQuiver->Update(mMatrix, aDeltaTime);
		}

		//当たり判定.
		OnCollision();
	}
	void Player::Draw3D(void)
	{
		Get3D().SetWorldMatrix(&mMatrix);
		if (mpHitPoint->IsEmpty()) { return; }

		//無敵状態かどうか.
		if (mIsInvincible) {

			if (!mInvincibleTime.GetWaitTime("Switch").IsExceed(true)) {
				mpModel->Draw();
				//子、プレイヤーが持つオブジェクトを描画.
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

			//子、プレイヤーが持つオブジェクトを描画.
			mpChild->Draw();
			mpBow->Draw3D();
			mpQuiver->Draw3D();
		}
	}
	void Player::MoveAction(float aDeltaTime)
	{
		auto camera = Get3D().GetCamera();
		auto camRot = camera.GetCamRot();

		//アニメーションリセット.
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
		//動いている時だけカベずり.
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

		//構えていない,かつ,移動量がオール0の場合.
		if (!mIsBendBow && velocity == D3DXVECTOR3(0.0f, 0.0f, 0.0f)) { return; }

		//移動時のサウンド.
		if (mIsMove) {
			//弓を構えているか.
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


		//弓を構えているときは照準の方向に向く.
		//斜め移動をした際に移動速度が上昇しないようにする.
		if (mIsBendBow || mIsMove &&
			(velocity.x != 0.0f || velocity.z != 0.0f)) {
			D3DXVec3Normalize(&velocity, &velocity);

			//弓を構えている時の速度.
			constexpr float BendSpeed = 2.5f;
			if (mIsBendBow) { velocity *= BendSpeed; }
			else { velocity *= MoveSpeed; }

			AddPosition(velocity * aDeltaTime);

			D3DXVECTOR3 playerForward{ 0.0f,0.0f,1.0f };
			D3DXVec3TransformNormal(&playerForward, &playerForward, &mRotateMatrix);

			//回転角度
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
				//回転方向
				D3DXVECTOR3 cross;
				if (mIsBendBow) { D3DXVec3Cross(&cross, &playerForward, &centerVec); }
				else { D3DXVec3Cross(&cross, &playerForward, &velocity); }

				//符号反転.
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
		//ホイール(上下関係なし)で切り替え.
		if (GetInput().GetMouseWheel()) {
			if (mWaitTimeManager.IsRegistered("Switch")) {
				//遅延時間なら変更処理を行わない
				if (!mWaitTimeManager.GetWaitTime("Switch").IsExceed(true)) {
					return;
				}
			}
			//選択されているタイプによって切り替える.
			switch (mSelectedArrowType)
			{
			case ArrowType::eNormalArrow:
				mSelectedArrowType = ArrowType::eFireArrow;
				break;
			case ArrowType::eFireArrow:
				mSelectedArrowType = ArrowType::eNormalArrow;
				break;
			default:
				_ASSERT_EXPR(false, L"登録されていないタイプが渡されました。");
				break;
			}
		}
	}

	bool Player::NumberOfSelectArrows(void)
	{
		if (mSelectedArrowType == ArrowType::eNormalArrow) {
			//残数１以上.
			return true;
		}
		else if (mSelectedArrowType == ArrowType::eFireArrow) {
			if (!mpNumOfArrowsForFire->IsEmpty()) {
				//残数１以上.
				return true;
			}
		}
		//残数0.
		return false;
	}

	void Player::AttackAction(void)
	{
		static bool isDistanceDelay = false;
		//次に構えられるまでの時間(60=約1秒).
		static int waitTime = 60 * 2;

		//強制的に構えを終えた場合は一定時間構えることができない.
		if (isDistanceDelay) {
			waitTime--;
			if (waitTime < 0) {
				waitTime = 60 * 2;
				isDistanceDelay = false;
			}
		}

		static bool isShotDelay = false;
		static float WaitTime = 60.0f * 0.5f;
		//矢を発射した後すぐに発射できないようにする.
		if (isShotDelay) {
			WaitTime--;
			if (WaitTime < 0.0f) {
				WaitTime = 60.0f * 0.5f;
				isShotDelay = false;
			}
		}

		//スタミナが0以上であり、かつ、強制的に終了されていなければ(時間が経っていれば).
		if (GetInput().GetKey(VK_RBUTTON) && !mpStamina->IsEmpty() && !isDistanceDelay) {
			{//カメラ制御.
				D3DXVECTOR3 camTransPos(0.7f, 0.5f, 1.75f);
				mpScene->GetCamera()->SetTransPosition(camTransPos, 10);
				mpScene->GetCamera()->ZoomIn();
				mpScene->GetCamera()->BendSensitivity();
			}
			//フラグ変更.
			mIsBendBow = true;
			{//クロスヘアの拡縮.
				auto object = mpScene->FindGameObject(eUserInterface);
				if (!object) { return; }
				auto userinterface = object->Cast<UserInterface>(eUserInterface);
				if (!userinterface) { return; }
				userinterface->CrossHairScaleDown();
			}

			//矢の生成、発射等.
			if (GetInput().GetKey(VK_LBUTTON)) {
				if (NumberOfSelectArrows()) { mIsShootArrow = true; }
			}
			else {
				mIsShootArrow = false;
			}

			//前に撃った矢から一定時間たっていれば(連続発射を防ぐため).
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
			//スタミナが0以下なら.
			//強制的にカメラを戻す.
			mpScene->GetCamera()->SetTransPosition({ 0.0f,1.0f,0.0f }, 10);
			mpScene->GetCamera()->ZoomOut();
			mpScene->GetCamera()->NormalSensitivity();

			//アニメーションリセット.
			mpChild->AllAnimeFrameReset();

			//強制的にフラグ変更.
			mIsBendBow = false;
			isDistanceDelay = true;
			mIsShootArrow = false;
		}

		//構えをやめたとき.
		if (GetInput().GetKeyUp(VK_RBUTTON)) {
			mpScene->GetCamera()->SetTransPosition({ 0.0f,1.0f,0.0f }, 10);
			mpScene->GetCamera()->ZoomOut();
			mpScene->GetCamera()->NormalSensitivity();

			//子のアニメーションをリセット.
			mpChild->AllAnimeFrameReset();

			//フラグ変更.
			mIsBendBow = false;
			isShotDelay = false;
			mIsShootArrow = false;
		}
	}

	void Player::StaminaPlus(void)
	{
		constexpr float AddValue = 0.15f;
		//矢を放とうとしていなければ.
		if (!mIsShootArrow) { mpStamina->Add(AddValue); }
	}

	void Player::StaminaMinus(void)
	{
		constexpr float RemoveValue = 0.2f;
		//弓を構えて、矢を引いていれば減少.
		if (mIsShootArrow) { mpStamina->Remove(RemoveValue); }
	}

	void Player::StaminaManagement(void)
	{
		//スタミナ減少.
		StaminaMinus();

		//スタミナ回復.
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

			//空でなければ.
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
					//地面に埋まるため高さ(y)には影響なし
					D3DXMatrixTranslation(&tmpMat, vec.x, 0.0f, vec.z);
					mTransMatrix *= tmpMat;
				}
			}
		}
	}
}
