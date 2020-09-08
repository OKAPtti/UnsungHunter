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
		{//各頂点設定.
			//座標.
			mVertexList[0].mPos = { -0.1f,0.1f,0.0f };
			mVertexList[1].mPos = { 0.1f,0.1f,0.0f };
			mVertexList[2].mPos = { -0.1f,-0.1f,0.0f };
			mVertexList[3].mPos = { 0.1f,-0.1f,0.0f };

			//色(255,255,255,255).
			mVertexList[0].mColor = mVertexList[1].mColor = mVertexList[2].mColor = mVertexList[3].mColor = -1;

			//UV.
			mVertexList[0].mUV = { 0.0f,0.0f };
			mVertexList[1].mUV = { 1.0f,0.0f };
			mVertexList[2].mUV = { 0.0f,1.0f };
			mVertexList[3].mUV = { 1.0f,1.0f };
		}

		//画像読み込み.
		mpTexture = GetTexMgr().GetTexture("Jet.bmp");
	}

	void Effect::UpdateTransform(float aDeltaTime, const D3DXMATRIX& aMatrix)
	{
		//エフェクトで使うログ.
		mLogList.push_back(aMatrix);

		//指定されたサイズを超えた場合、先頭から削除.
		if (mLogList.size() > static_cast<UINT>(mMaxListSize)) { mLogList.pop_front(); }
	}
	void Effect::Draw3D(void)
	{
		//描画する数.
		const int SpriteNum = (mLogList.size() - 1);
		if (SpriteNum <= 0) { return; }

		//デバイス関連.
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
				//左上、右上、左下、右下.
				D3DXVECTOR3{-0.1f,0.0f,0.0f},{0.1f,0.0f,0.0f},{-0.1f,0.0f,0.0f},{0.1f,0.0f,0.0f}
			};

			//雲画像の底辺の座標
			for (int i = 2; i < 4; i++) {
				D3DXVec3TransformCoord(&mVertexList[i].mPos, &shiftList[i], &(*it));
				mVertexList[i].mUV.y = (CastFloat(spriteCnt) / CastFloat(SpriteNum));
			}

			it++;

			//ログ行列が最後に到達したか.
			if (it == mLogList.end()) { break; }

			//雲画像の上辺の座標
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
		//空でない場合先頭から順に削除.
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