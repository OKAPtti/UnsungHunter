#include "ArsCameraSystem.h"
#include "../../GamaObjects/BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	CameraSystem::CameraSystem(void)
		: mpTargetViewpoint()
		, mViewpoint(0.0f, 0.0f, 0.0f)
		, mViewMatrix()
		, mCamRot()
		, mCamAng(0.0f, -0.5f)
		, mTransPosition(0.0f, 1.0f, 0.0f)
		, mTransPositionLerper()
		, mSensitivity(1.0f)
		, mAdjustmentValue(0.001f)
		, mViewingAngle(60.0f)
		, mAngleLerper()
	{
		D3DXMatrixIdentity(&mViewMatrix);
		D3DXMatrixIdentity(&mCamRot);
	}


	CameraSystem::~CameraSystem(void)
	{
	}

	void CameraSystem::UpdateRotation(const D3DXVECTOR2& aAddRadian)
	{
		mCamAng += (aAddRadian * (mSensitivity * mAdjustmentValue));

		//Y座標の角度を下限と上限内に留める
		constexpr float AngleDegreeLimit = D3DXToRadian(50.0f);
		if (fabsf(mCamAng.y) > AngleDegreeLimit) {
			if (mCamAng.y > 0) { mCamAng.y = AngleDegreeLimit; }
			else { mCamAng.y = -AngleDegreeLimit; }
		}

		constexpr float PI = 3.141593f;
		constexpr float RoundRadian = PI * 2.0f;

		//未満・超過を補正
		if (mCamAng.x < 0) { mCamAng.x += RoundRadian; }
		if (mCamAng.x >= RoundRadian) { mCamAng.x -= RoundRadian; }

		D3DXMATRIX tmpMat;
		D3DXMatrixRotationX(&mCamRot, mCamAng.y);
		D3DXMatrixRotationY(&tmpMat, mCamAng.x);
		mCamRot *= tmpMat;

		//カメラ位置.
		if (!mTransPositionLerper.IsMaxCount()) {
			mTransPositionLerper.AddCount();
			mTransPosition = mTransPositionLerper.Lerp();
		}

		//視野角.
		if (!mAngleLerper.IsMaxCount()) {
			mAngleLerper.AddCount();
			mViewingAngle = mAngleLerper.Lerp();
		}
		if (mpTargetViewpoint.expired()) {
			constexpr float BaseAngle = 60.0f;
			mViewingAngle = BaseAngle;
		}
	}

	void CameraSystem::UpdateViewMatrix(void)
	{
		//カメラの位置.
		D3DXVECTOR3 camPos{ 0.0f,1.5f,-3.0f };
		D3DXVec3TransformCoord(&camPos, &camPos, &mCamRot);

		D3DXVECTOR3 camView = GetViewpoint();

		camPos += camView;

		D3DXVECTOR3 viewPos = GetViewpoint();

		D3DXMATRIX viewMat;
		D3DXMatrixLookAtLH(&mViewMatrix,
			&camPos,	// カメラの位置
			&viewPos,	// カメラの視点
			&D3DXVECTOR3(0.0f, 1.0f, 0.0f)	// カメラの頭の方向
		);
	}

	const D3DXMATRIX& CameraSystem::GetViewMatrix(void)const
	{
		return mViewMatrix;
	}

	const D3DXVECTOR3& CameraSystem::GetViewpoint(void)
	{
		if (!mpTargetViewpoint.expired()) {
			auto pGameObject = mpTargetViewpoint.lock();
			if (pGameObject->GetIsAlive()) {

				//矢を放つとき、カメラを近づけた際に左右にマウスを動かしたときに
				//プレイヤーが左側にいるようにする.
				D3DXVECTOR3 xVec{ 1.0f,0.0f,0.0f };
				D3DXMATRIX invViewMat;
				D3DXMatrixInverse(&invViewMat, nullptr, &mViewMatrix);

				D3DXVec3TransformNormal(&xVec, &xVec, &invViewMat);

				xVec = xVec * mTransPosition.x;

				D3DXVECTOR3 tmpVec = D3DXVECTOR3(0.0f, mTransPosition.y, mTransPosition.z) + xVec;

				D3DXVec3TransformNormal(&tmpVec, &mTransPosition, &invViewMat);

				//対象物の位置を取得.
				const D3DXVECTOR3 TargetPos = pGameObject->GetPosition();

				mViewpoint = TargetPos + tmpVec;
			}
			else {
				//Killされていたら手放す
				mpTargetViewpoint.reset();
			}
		}
		return mViewpoint;
	}
	const D3DXMATRIX& CameraSystem::GetCamRot(void)
	{
		return mCamRot;
	}

	const D3DXVECTOR3& CameraSystem::GetCenterVector(void) const
	{
		const D3DXMATRIX InvMat = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &mViewMatrix);

		return *D3DXVec3Normalize(&D3DXVECTOR3(), &(mViewpoint - D3DXVECTOR3{ InvMat._41, InvMat._42, InvMat._43 }));
	}

	float CameraSystem::GetViewingAngle(void) const
	{
		return mViewingAngle;
	}

	void CameraSystem::SetTransPosition(const D3DXVECTOR3& aTransPosition, int aEndCount)
	{
		mTransPositionLerper.Reset();
		mTransPositionLerper.SetStart(mTransPosition);
		mTransPositionLerper.SetEnd(aTransPosition);
		mTransPositionLerper.SetEndCount(aEndCount);
	}

	void CameraSystem::ZoomIn(void)
	{
		constexpr float MaxAngle = 40.0f;
		constexpr int EndCount = 10;
		mAngleLerper.Reset();
		mAngleLerper.SetStart(mViewingAngle);
		mAngleLerper.SetEnd(MaxAngle);
		mAngleLerper.SetEndCount(EndCount);
	}
	void CameraSystem::ZoomOut(void)
	{
		constexpr float MinAngle = 60.0f;
		constexpr int EndCount = 10;
		mAngleLerper.Reset();
		mAngleLerper.SetStart(mViewingAngle);
		mAngleLerper.SetEnd(MinAngle);
		mAngleLerper.SetEndCount(EndCount);
	}
}