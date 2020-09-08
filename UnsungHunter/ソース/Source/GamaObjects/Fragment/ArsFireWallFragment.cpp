#include "ArsFireWallFragment.h"
#include "../../Core/Camera/ArsCameraSystem.h"
#include "../../Scene/BaseScene/ArsScene.h"
#include "../../Core/Texture/ArsTextureManager.h"
#include "../BaseGameObjects/ArsGameObject.h"
#include "../Character/Enemy/ArsWolf.h"
#include "../../Core/DirectX/ArsDirectGraphics.h"

namespace Alrescha {

	FireWallFragment::FireWallFragment(void)
		: GameObject(eFireWallFragment)
		, mWaitTimeManager()
		, mVertex()
		, mSubMatrix()
		, mTextureNumber(0)
	{
		D3DXMatrixIdentity(&mSubMatrix);
	}

	FireWallFragment::~FireWallFragment(void)
	{}

	void FireWallFragment::Initialize(const D3DXVECTOR3& aPosition)
	{
		GameObject::Initialize();
		mpTexture = GetTexMgr().GetTexture("FireWall.png");

		D3DXMatrixTranslation(&mTransMatrix, aPosition.x, aPosition.y, aPosition.z);

		mVertex[0].mPos = { -0.5f,5.0f,0.0f };
		mVertex[1].mPos = { 0.5f,5.0f,0.0f };
		mVertex[2].mPos = { 0.5f,0.0f,0.0f };
		mVertex[3].mPos = { -0.5f,0.0f,0.0f };

		{//初期テクスチャの設定.
			int num = 0;

			//画像にコマがいくつ並んでるか
			const int ColNum = 8;
			const int RowNum = 8;
			//縦横のシーク位置(numに該当するコマの左上)
			int col = num % ColNum;
			int row = num / RowNum;
			//シーク位置と、その右下の座標
			float left = col / CastFloat(ColNum);
			float top = row / CastFloat(RowNum);
			float right = (col + 1) / CastFloat(ColNum);
			float bottom = (row + 1) / CastFloat(RowNum);

			mVertex[0].mUV = { left,top };
			mVertex[1].mUV = { right,top };
			mVertex[2].mUV = { right,bottom };
			mVertex[3].mUV = { left , bottom };
		}

		{//WaitTime.
			//生成するまでの時間.
			auto& waitTime = mWaitTimeManager.GetWaitTime("Delay");
			waitTime.SetWaitTime(0.1f);

			//削除する時間.
			mWaitTimeManager.Register("Delete", 10.0f);
		}

		//色指定.
		for (auto& it : mVertex) {
			it.mColor = D3DCOLOR_ARGB(190,255,255,255);
		}

		{//地面の高さに合わせた場所に描画する.
			const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
			auto info = mpScene->RayTest(GetPosition(), UnderDirection, eTerrain);
			//メッシュまでの距離(高さ)を引く.
			//炎より地面が下にある場合.
			if (info.mDistance > 0.0f) {
				AddPosition({ 0.0f,-info.mDistance,0.0f });
			}
			//炎より地面が上にある場合.
			else{
				const D3DXVECTOR3 UpwardDirection(0.0f, 1.0f, 0.0f);
				info = mpScene->RayTest(GetPosition(), UpwardDirection, eTerrain);
				AddPosition({ 0.0f,info.mDistance,0.0f });
			}
		}
	}

	void FireWallFragment::Update(float aDeltaTime)
	{
		UpdateAnimation();

		mWaitTimeManager.Update(aDeltaTime);

		//ビルボード.
		//カメラの逆行列.
		const auto& InvCamMat = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &(mpScene->GetCamera()->GetViewMatrix()));;
		//逆行列から回転行列だけ取り出す.
		mRotateMatrix._11 = InvCamMat._11; mRotateMatrix._12 = InvCamMat._12; mRotateMatrix._13 = InvCamMat._13;
		mRotateMatrix._31 = InvCamMat._31; mRotateMatrix._32 = InvCamMat._32; mRotateMatrix._33 = InvCamMat._33;

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;

		D3DXMATRIX rotate;
		D3DXMatrixRotationY(&rotate, D3DXToRadian(90.0f));
		mSubMatrix = rotate * mMatrix;

		Delete();
	}

	void FireWallFragment::Draw3D(void)
	{
		auto pDevice = Get3D().GetDevice();

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		pDevice->SetTexture(0, mpTexture->GetDirectTexture());
		Get3D().SetWorldMatrix(&mMatrix);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, mVertex, sizeof(Vertex));
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
	bool FireWallFragment::HitCheckRayPoly(const InputRayPoly& in, OutputRayPoly& out)
	{
		D3DXVECTOR3 vPos[4];
		D3DXVec3TransformCoord(&vPos[0], &mVertex[0].mPos, &mMatrix);
		D3DXVec3TransformCoord(&vPos[1], &mVertex[1].mPos, &mMatrix);
		D3DXVec3TransformCoord(&vPos[2], &mVertex[2].mPos, &mMatrix);
		D3DXVec3TransformCoord(&vPos[3], &mVertex[3].mPos, &mMatrix);

		//描画している炎の当たり判定.
		if (D3DXIntersectTri(&vPos[0], &vPos[1], &vPos[2], &in.mRayPos, &in.mRayDir, NULL, NULL, &out.mDistance) ||
			D3DXIntersectTri(&vPos[0], &vPos[2], &vPos[3], &in.mRayPos, &in.mRayDir, NULL, NULL, &out.mDistance)) {
			out.mIsHit = true;
		}
		else {
			out.mIsHit = false;
		}

		//当たっていれば.
		if (out.mIsHit) { return out.mIsHit; }

		//描画していない炎の当たり判定(炎の壁と平行にカメラが向いているときに隙間を埋めるため).
		D3DXVec3TransformCoord(&vPos[0], &mVertex[0].mPos, &mSubMatrix);
		D3DXVec3TransformCoord(&vPos[1], &mVertex[1].mPos, &mSubMatrix);
		D3DXVec3TransformCoord(&vPos[2], &mVertex[2].mPos, &mSubMatrix);
		D3DXVec3TransformCoord(&vPos[3], &mVertex[3].mPos, &mSubMatrix);

		if (D3DXIntersectTri(&vPos[0], &vPos[1], &vPos[2], &in.mRayPos, &in.mRayDir, NULL, NULL, &out.mDistance) ||
			D3DXIntersectTri(&vPos[0], &vPos[2], &vPos[3], &in.mRayPos, &in.mRayDir, NULL, NULL, &out.mDistance)) {
			out.mIsHit = true;
		}
		else {
			out.mIsHit = false;
		}

		return out.mIsHit;
	}

	void FireWallFragment::UpdateAnimation(void)
	{
		if (mWaitTimeManager.IsRegistered("Delay")) {
			//遅延時間なら生成処理を行わない
			if (!mWaitTimeManager.GetWaitTime("Delay").IsExceed(true)) {
				return;
			}
		}

		mTextureNumber++;
		//画像にコマがいくつ並んでるか
		constexpr int ColNum = 8;
		constexpr int RowNum = 8;

		constexpr int MaxTextureNumber = ColNum * RowNum;

		if (mTextureNumber > MaxTextureNumber) { mTextureNumber = 0; }

		//縦横のシーク位置(numに該当するコマの左上)
		int col = mTextureNumber % ColNum;
		int row = mTextureNumber / RowNum;
		//シーク位置と、その右下の座標
		float left = col / static_cast<float>(ColNum);
		float top = row / static_cast<float>(RowNum);
		float right = (col + 1) / static_cast<float>(ColNum);
		float bottom = (row + 1) / static_cast<float>(RowNum);

		mVertex[0].mUV = { left ,top };
		mVertex[1].mUV = { right,top };
		mVertex[2].mUV = { right,bottom };
		mVertex[3].mUV = { left , bottom };
	}
	void FireWallFragment::Delete(void)
	{
		if (mWaitTimeManager.IsRegistered("Delete")) {
			//削除時間が経っていなければ削除処理を行わない
			if (!mWaitTimeManager.GetWaitTime("Delete").IsExceed(true)) {
				return;
			}
		}

		//削除(Sceneのリストからも削除).
		Kill();
	}
}