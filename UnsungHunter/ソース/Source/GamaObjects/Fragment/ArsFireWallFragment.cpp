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

		{//�����e�N�X�`���̐ݒ�.
			int num = 0;

			//�摜�ɃR�}����������ł邩
			const int ColNum = 8;
			const int RowNum = 8;
			//�c���̃V�[�N�ʒu(num�ɊY������R�}�̍���)
			int col = num % ColNum;
			int row = num / RowNum;
			//�V�[�N�ʒu�ƁA���̉E���̍��W
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
			//��������܂ł̎���.
			auto& waitTime = mWaitTimeManager.GetWaitTime("Delay");
			waitTime.SetWaitTime(0.1f);

			//�폜���鎞��.
			mWaitTimeManager.Register("Delete", 10.0f);
		}

		//�F�w��.
		for (auto& it : mVertex) {
			it.mColor = D3DCOLOR_ARGB(190,255,255,255);
		}

		{//�n�ʂ̍����ɍ��킹���ꏊ�ɕ`�悷��.
			const D3DXVECTOR3 UnderDirection(0.0f, -1.0f, 0.0f);
			auto info = mpScene->RayTest(GetPosition(), UnderDirection, eTerrain);
			//���b�V���܂ł̋���(����)������.
			//�����n�ʂ����ɂ���ꍇ.
			if (info.mDistance > 0.0f) {
				AddPosition({ 0.0f,-info.mDistance,0.0f });
			}
			//�����n�ʂ���ɂ���ꍇ.
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

		//�r���{�[�h.
		//�J�����̋t�s��.
		const auto& InvCamMat = *D3DXMatrixInverse(&D3DXMATRIX(), nullptr, &(mpScene->GetCamera()->GetViewMatrix()));;
		//�t�s�񂩂��]�s�񂾂����o��.
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

		//�`�悵�Ă��鉊�̓����蔻��.
		if (D3DXIntersectTri(&vPos[0], &vPos[1], &vPos[2], &in.mRayPos, &in.mRayDir, NULL, NULL, &out.mDistance) ||
			D3DXIntersectTri(&vPos[0], &vPos[2], &vPos[3], &in.mRayPos, &in.mRayDir, NULL, NULL, &out.mDistance)) {
			out.mIsHit = true;
		}
		else {
			out.mIsHit = false;
		}

		//�������Ă����.
		if (out.mIsHit) { return out.mIsHit; }

		//�`�悵�Ă��Ȃ����̓����蔻��(���̕ǂƕ��s�ɃJ�����������Ă���Ƃ��Ɍ��Ԃ𖄂߂邽��).
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
			//�x�����ԂȂ琶���������s��Ȃ�
			if (!mWaitTimeManager.GetWaitTime("Delay").IsExceed(true)) {
				return;
			}
		}

		mTextureNumber++;
		//�摜�ɃR�}����������ł邩
		constexpr int ColNum = 8;
		constexpr int RowNum = 8;

		constexpr int MaxTextureNumber = ColNum * RowNum;

		if (mTextureNumber > MaxTextureNumber) { mTextureNumber = 0; }

		//�c���̃V�[�N�ʒu(num�ɊY������R�}�̍���)
		int col = mTextureNumber % ColNum;
		int row = mTextureNumber / RowNum;
		//�V�[�N�ʒu�ƁA���̉E���̍��W
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
			//�폜���Ԃ��o���Ă��Ȃ���΍폜�������s��Ȃ�
			if (!mWaitTimeManager.GetWaitTime("Delete").IsExceed(true)) {
				return;
			}
		}

		//�폜(Scene�̃��X�g������폜).
		Kill();
	}
}