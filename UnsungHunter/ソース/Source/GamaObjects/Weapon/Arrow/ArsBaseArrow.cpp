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

		//発射位置.
		D3DXMatrixTranslation(&mTransMatrix, aShotPosition.x, aShotPosition.y, aShotPosition.z);


		//発射する角度.
		{
			const auto CurrentPosition = GetPosition();

			//向きたい方向の座標.
			const auto ShootPosition = CurrentPosition + aShootAngle;

			//ポジション方向を向いた時の、ワールドから見た各軸のベクトル
			D3DXVECTOR3 localX, localY, localZ;
			//Z軸の方向(自分から見た相手の向き)
			D3DXVec3Normalize(&localZ, &(ShootPosition - CurrentPosition));
			//X軸の方向(Z軸とワールドY軸に垂直な軸)
			D3DXVec3Cross(&localX, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &localZ);
			D3DXVec3Normalize(&localX, &localX);
			//Y軸の方向(X軸とZ軸に垂直な軸)
			D3DXVec3Cross(&localY, &localZ, &localX);
			D3DXVec3Normalize(&localY, &localY);

			D3DXMATRIX m;
			D3DXMatrixIdentity(&m);
			m._11 = localX.x; m._12 = localX.y; m._13 = localX.z;
			m._21 = localY.x; m._22 = localY.y; m._23 = localY.z;
			m._31 = localZ.x; m._32 = localZ.y; m._33 = localZ.z;

			mRotateMatrix = m;
		}
		//発射する速度.
		mShootPower = aPower;

		{//WaitTime.
			auto& waitTime = mWaitTimeManager.GetWaitTime("Delay");
			waitTime.SetWaitTime(0.00125f);
		}

		//エフェクトの生成.
		mpEffect = std::make_unique<Effect>(eFireArrow);
		mpEffect->Initialize();

		//発射時に一度だけ音を鳴らす.
		GetSoundManager().Play("ShotSound");
	}

	void BaseArrow::Update(float aDeltaTime)
	{
		if (mIsHit) {
			//先頭から徐々に削除.
			mpEffect->RemoveLogList();

			//もし監視対象が死んでいたら矢も消す.
			if (mpHitObject.expired()) { Kill(); return; }
			//追尾.
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
		//当たった相手がキャラクターで無い場合は必要ない.
		if (!IsCharacter(pHitEntity->GetType())) { return; }
		//回転.
		const auto RotMat = mLocalRotate * pHitEntity->GetRotateMatrix();
		mRotateMatrix = RotMat;

		//座標.
		const auto LocalPos = *D3DXVec3TransformNormal(&D3DXVECTOR3(), &mLocalPosition, &mRotateMatrix);
		const auto Pos = LocalPos + pHitEntity->GetPosition();

		D3DXMatrixTranslation(&mTransMatrix, Pos.x, Pos.y, Pos.z);
	}
	void BaseArrow::MoveAction(float aDeltaTime)
	{
		//進行方向.
		D3DXVECTOR3 vel{ 0.0f,0.0f,1.0f };
		D3DXVec3TransformNormal(&vel, &vel, &mRotateMatrix);
		vel *= mShootPower;

		D3DXMATRIX tmpMat;
		D3DXMatrixTranslation(&tmpMat, vel.x, vel.y, vel.z);
		mTransMatrix = tmpMat * mTransMatrix;

		if (mWaitTimeManager.IsRegistered("Delay")) {
			//遅延時間ならログ処理を行わない
			if (!mWaitTimeManager.GetWaitTime("Delay").IsExceed()) {
				return;
			}
		}

		//オブジェクトに当たっていればエフェクト更新不要.
		if (mIsHit) { return; }

		mpEffect->UpdateTransform(aDeltaTime, mMatrix);
	}
	void BaseArrow::OnCollision(void)
	{
		//矢がどこかに当たっていればレイ判定する必要が無い.
		if (mIsHit) { return; }

		//レイの発射方向ベクトル.
		D3DXVECTOR3 rayDir(0.0f, 0.0f, 1.0f);
		D3DXVec3TransformNormal(&rayDir, &rayDir, &mRotateMatrix);
		//当たったメッシュとの距離.
		float distance = 0.0f;

		//レイの発射位置を矢羽に移動.
		//矢じりから飛ばすと貫通してしまうため発射位置を矢羽にする.
		D3DXVECTOR3 arrowFeathers(0.0f, 0.0f, -0.45f);
		D3DXVec3TransformNormal(&arrowFeathers, &arrowFeathers, &mRotateMatrix);
		//矢羽から矢じりまでの距離.
		constexpr float ArrowheadDistance = 1.0f;
		RayPosition = GetPosition() + arrowFeathers;

		//レイ判定で必要な情報を格納.
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
						//当たった相手を保持.
						mpHitObject = it->shared_from_this();
						auto pEnemy = it->Cast<Enemy>(eWolf);
						if (!pEnemy) { continue; }

						//ノックバックさせるベクトルの生成.
						rayDir.y = 0;
						D3DXVec3Normalize(&rayDir, &rayDir);
						//吹き飛ばす高さ.
						constexpr float ImpactY = 0.5f;
						rayDir.y = ImpactY;
						//吹き飛ばす力.
						rayDir *= 0.25f;
						pEnemy->SetImpactDirection(rayDir);

						//矢じりの先端に移動(補正)する.
						D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
						D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
						//rayDir:ノックバックで移動した分(Yを除いて)をプラス.
						D3DXVECTOR3 knockVec = { rayDir.x,0.0f,rayDir.z };
						const auto MyPos = GetPosition() + shift + knockVec;
						D3DXMatrixTranslation(&mTransMatrix, MyPos.x, MyPos.y, MyPos.z);

						//Wolfのエンティティの座標から見た相対的な位置.
						mLocalPosition = MyPos - it->GetPosition();
						D3DXMATRIX invMat;
						D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
						D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);


						//逆行列.
						D3DXMatrixInverse(&mLocalRotate, nullptr, &it->GetRotateMatrix());
						//ウルフの回転行列をかけてやれば、矢が刺さった角度になる
						//ウルフが回転すれば矢も回転する.
						mLocalRotate *= mRotateMatrix;

						//ダメージ量と攻撃名を伝える.
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
						//当たった相手を保持.
						mpHitObject = it->shared_from_this();

						//矢じりの先端に移動(補正)する.
						D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
						D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
						const auto MyPos = GetPosition() + shift;
						D3DXMatrixTranslation(&mTransMatrix, MyPos.x, MyPos.y, MyPos.z);

						mLocalPosition = MyPos - it->GetPosition();
						D3DXMATRIX invMat;
						D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
						D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);

						//逆行列.
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
					//当たった相手を保持.
					mpHitObject = terrain->shared_from_this();

					//矢じりの先端に移動(補正)する.
					D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
					D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
					const auto MyPos = GetPosition() + shift;

					//Terrainのエンティティの座標から見た相対的な位置.
					mLocalPosition = MyPos - terrain->GetPosition();
					D3DXMATRIX invMat;
					D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
					D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);

					//逆行列.
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

			//HPが０以下なら当たり判定をしない.
			//この処理がないと透明になってGameObjectが削除されるまでの間矢が突き刺さってしまう.
			if (boss->GetHp()->IsEmpty()) { return; }

			if (boss->HitCheckRay(in, out)) {
				if (out.mDistance <= ArrowheadDistance) {
					mIsHit = true;
					//当たった相手を保持.
					mpHitObject = boss->shared_from_this();

					//ノックバックさせるベクトルの生成.
					rayDir.y = 0;
					D3DXVec3Normalize(&rayDir, &rayDir);
					//吹き飛ばす高さ.
					constexpr float ImpactY = 0.5f;
					rayDir.y = ImpactY;
					//吹き飛ばす力.
					rayDir *= 0.25f;
					boss->SetImpactDirection(rayDir);

					//矢じりの先端に移動(補正)する.
					D3DXVECTOR3 shift = { 0.0f,0.0f,-(ArrowheadDistance - out.mDistance) };
					D3DXVec3TransformCoord(&shift, &shift, &mRotateMatrix);
					//rayDir:ノックバックで移動した分をプラス.
					D3DXVECTOR3 knockVec = { rayDir.x,rayDir.y,rayDir.z };
					const auto MyPos = GetPosition() + shift + knockVec;
					D3DXMatrixTranslation(&mTransMatrix, MyPos.x, MyPos.y, MyPos.z);

					//Bossのエンティティの座標から見た相対的な位置.
					mLocalPosition = MyPos - boss->GetPosition();
					D3DXMATRIX invMat;
					D3DXMatrixInverse(&invMat, nullptr, &mRotateMatrix);
					D3DXVec3TransformNormal(&mLocalPosition, &mLocalPosition, &invMat);


					//逆行列.
					D3DXMatrixInverse(&mLocalRotate, nullptr, &boss->GetRotateMatrix());
					//ウルフの回転行列をかけてやれば、矢が刺さった角度になる
					//ウルフが回転すれば矢も回転する.
					mLocalRotate *= mRotateMatrix;

					//ダメージ量と攻撃名を伝える.
					boss->Damage(mDamage, "ArrowDamage");
					boss->Hit();
				}
			}
		}
	}
}