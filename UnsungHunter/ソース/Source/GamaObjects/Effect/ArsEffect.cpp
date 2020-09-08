#include "ArsEffect.h"
#include "../../Core/DirectX/ArsDirectGraphics.h"
#include "../../Core/Texture/ArsTextureManager.h"

namespace Alrescha {
	Effect::Effect(int aType)
		: GameObject(aType)
		, mLogList()
		, mVertexList()
		, mMaxListSize(20)
	{}
	Effect::~Effect(void)
	{}

	void Effect::Initialize(void)
	{
		{//�e���_�ݒ�.
			//���W.
			mVertexList[0].mPos = { -0.1f,0.1f,0.0f };
			mVertexList[1].mPos = { 0.1f,0.1f,0.0f };
			mVertexList[2].mPos = { -0.1f,-0.1f,0.0f };
			mVertexList[3].mPos = { 0.1f,-0.1f,0.0f };

			//�F(255,255,255,255).
			mVertexList[0].mColor = mVertexList[1].mColor = mVertexList[2].mColor = mVertexList[3].mColor = -1;

			//UV.
			mVertexList[0].mUV = { 0.0f,0.0f };
			mVertexList[1].mUV = { 1.0f,0.0f };
			mVertexList[2].mUV = { 0.0f,1.0f };
			mVertexList[3].mUV = { 1.0f,1.0f };
		}

		//�摜�ǂݍ���.
		mpTexture = GetTexMgr().GetTexture("Jet.bmp");
	}

	void Effect::UpdateTransform(float aDeltaTime, const D3DXMATRIX& aMatrix)
	{
		//�G�t�F�N�g�Ŏg�����O.
		mLogList.push_back(aMatrix);

		//�w�肳�ꂽ�T�C�Y�𒴂����ꍇ�A�擪����폜.
		if (mLogList.size() > static_cast<UINT>(mMaxListSize)) { mLogList.pop_front(); }
	}
	void Effect::Draw3D(void)
	{
		//�`�悷�鐔.
		const int SpriteNum = (mLogList.size() - 1);
		if (SpriteNum <= 0) { return; }

		//�f�o�C�X�֘A.
		auto pDevice = Get3D().GetDevice();
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		pDevice->SetTexture(0, mpTexture->GetDirectTexture());
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);
		pDevice->SetTransform(D3DTS_WORLD, &matrix);

		int spriteCnt = 1;
		auto it = mLogList.begin();
		while (true) {
			std::array<D3DXVECTOR3, 4> shiftList{
				//����A�E��A�����A�E��.
				D3DXVECTOR3{-0.1f,0.0f,0.0f},{0.1f,0.0f,0.0f},{-0.1f,0.0f,0.0f},{0.1f,0.0f,0.0f}
			};

			//�_�摜�̒�ӂ̍��W
			for (int i = 2; i < 4; i++) {
				D3DXVec3TransformCoord(&mVertexList[i].mPos, &shiftList[i], &(*it));
				mVertexList[i].mUV.y = (CastFloat(spriteCnt) / CastFloat(SpriteNum));
			}

			it++;

			//���O�s�񂪍Ō�ɓ��B������.
			if (it == mLogList.end()) { break; }

			//�_�摜�̏�ӂ̍��W
			for (int i = 0; i < 2; i++) {
				D3DXVec3TransformCoord(&mVertexList[i].mPos, &shiftList[i], &(*it));
				mVertexList[i].mUV.y = (CastFloat(spriteCnt + 1) / CastFloat(SpriteNum));
			}

			pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID)mVertexList.data(), sizeof(Vertex));

			spriteCnt++;
		}
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	void Effect::RemoveLogList(void)
	{
		//��łȂ��ꍇ�擪���珇�ɍ폜.
		if (!mLogList.empty()) { mLogList.pop_front(); }
	}
	void Effect::SetVertexPos(const D3DXVECTOR3& aPosition)
	{
		mVertexList[0].mPos = { -aPosition.x, aPosition.y,aPosition.z };
		mVertexList[1].mPos = { aPosition.x, aPosition.y,aPosition.z };
		mVertexList[2].mPos = { -aPosition.x,-aPosition.y,aPosition.z };
		mVertexList[3].mPos = { aPosition.x,-aPosition.y,aPosition.z };
	}
	void Effect::SetVertexColor(D3DCOLOR aColor)
	{
		mVertexList[0].mColor = mVertexList[1].mColor = mVertexList[2].mColor = mVertexList[3].mColor = aColor;
	}
}