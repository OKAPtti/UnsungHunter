#include "ArsFireArrow.h"
#include "../../../Scene/BaseScene/ArsScene.h"
#include "../../Fragment/ArsFireWall.h"
#include "../../Character/Enemy/AllEnemyIncludes.h"
#include "../../Effect/ArsEffect.h"

namespace Alrescha {
	FireArrow::FireArrow(void)
		: BaseArrow(eFireArrow)
		, mIsCreateFireWall(false)
		, mWaitTimeManager()
	{}
	FireArrow::~FireArrow(void)
	{}
	void FireArrow::Initialize(const D3DXVECTOR3 & aShootAngle, float aPower, const D3DXVECTOR3 & aShotPosition)
	{
		BaseArrow::Initialize(aShootAngle, aPower, aShotPosition);
		auto& waitTime = mWaitTimeManager.GetWaitTime("Kill");
		waitTime.SetWaitTime(3.0f);
		//ダメージ量セット.
		mDamage = 34;

		mpEffect->SetVertexColor(D3DCOLOR_ARGB(255, 255, 0, 0));

	}
	void FireArrow::Update(float aDeltaTime)
	{
		BaseArrow::Update(aDeltaTime);

		mWaitTimeManager.Update(aDeltaTime);

		//炎を生成していれば.
		if (mIsCreateFireWall) { return; }

		if (mIsHit) {
			//当たっている相手が地面であれば炎をだす.
			auto pObject = mpHitObject.lock();
			if (!pObject) { return; }
			if (pObject->GetType() == eTerrain) {
				//FireWallの生成と刺さった位置,回転を渡す.
				auto pFireWall = std::make_shared<FireWall>();
				pFireWall->Initialize(RayPosition);
				pFireWall->SetRotation(GetRotateMatrix());
				mpScene->AddGameObject(pFireWall, 4U);
				mIsCreateFireWall = true;
			}
			//当っている相手が敵であれば、継続ダメージを与える.
			else if (IsEnemy(pObject->GetType())) {
				if (mWaitTimeManager.IsRegistered("Kill")) {
					//削除時間でないなら削除処理はしない
					if (!mWaitTimeManager.GetWaitTime("Kill").IsExceed(true)) {
						//ダメージ.
						if (pObject->GetType() == eWolf) {
							auto pWolf = pObject->Cast<Wolf>(eWolf);
							pWolf->Damage(3, "FireArrow");
						}

						if (pObject->GetType() == eBoss) {
							auto pWolf = pObject->Cast<Boss>(eBoss);
							pWolf->Damage(3, "FireArrow");
						}


						return;
					}
				}
				Kill();
			}
		}
	}
	void FireArrow::Draw3D(void)
	{
		BaseArrow::Draw3D();
	}
	void FireArrow::OnCollision(void)
	{
		BaseArrow::OnCollision();
	}
}