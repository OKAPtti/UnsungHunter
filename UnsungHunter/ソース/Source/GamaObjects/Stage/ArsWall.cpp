#include "ArsWall.h"
#include "../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	Wall::Wall(void)
		: GameObject(eWall)
	{}
	Wall::~Wall(void)
	{}
	void Wall::Initialize(void)
	{
		GameObject::Initialize();
		mpModel = GetModelMgr().GetModel("Wall_01");
	}
	void Wall::Update(float aDeltaTime)
	{
		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
	}
	bool Wall::HitCheckRay(const InputRay& in, OutputRay& out)
	{
		D3DXMATRIX invMat;
		D3DXMatrixInverse(&invMat, nullptr, &mMatrix);

		D3DXVECTOR3 localPos, localVec;
		D3DXVec3TransformCoord(&localPos, &in.mRayPos, &invMat);

		D3DXVec3TransformNormal(&localVec, &in.mRayDir, &invMat);

		//当ったかどうか.
		BOOL isHit;
		//メッシュまでの距離.
		float distance;
		//レイが当たったポリゴン番号.
		DWORD polyNo;

		//カベのメッシュ.
		LPD3DXMESH wallMesh = mpModel->GetMesh();

		D3DXIntersect(wallMesh, &localPos, &localVec, &isHit, &polyNo, nullptr, nullptr, &distance, nullptr, nullptr);

		//当っていれば.
		if (isHit) {
			//注意!:WORD型限定.
			WORD* pI;
			wallMesh->LockIndexBuffer(0, (LPVOID*)&pI);

			//三角ポリゴンの頂点情報を一時的格納.
			DWORD vertexNo[3];
			vertexNo[0] = *(pI + polyNo * 3 + 0);
			vertexNo[1] = *(pI + polyNo * 3 + 1);
			vertexNo[2] = *(pI + polyNo * 3 + 2);

			wallMesh->UnlockIndexBuffer();

			//メッシュの各情報を格納.
			struct Vertex {
				D3DXVECTOR3 mPos;
				D3DXVECTOR3 mNormal;
				D3DXVECTOR2 mUV;
			};

			Vertex* vertex;

			wallMesh->LockVertexBuffer(0, (LPVOID*)&vertex);

			D3DXVECTOR3 vPos[3];
			//三角ポリゴンの座標情報を一時的格納.
			vPos[0] = (vertex + vertexNo[0])->mPos;
			vPos[1] = (vertex + vertexNo[1])->mPos;
			vPos[2] = (vertex + vertexNo[2])->mPos;

			wallMesh->UnlockVertexBuffer();

			D3DXVECTOR3 Vec1, Vec2;
			Vec1 = vPos[1] - vPos[0];
			Vec2 = vPos[2] - vPos[0];

			D3DXVECTOR3 WallVec;
			//法線を取得.
			D3DXVec3Cross(&WallVec, &Vec1, &Vec2);

			D3DXVec3TransformNormal(&WallVec, &WallVec, &mMatrix);

			D3DXVec3Normalize(&WallVec, &WallVec);

			float dot;
			dot = D3DXVec3Dot(&WallVec, &-in.mRayDir);//-in.RayDir = カメラの進行方向.
			float limit = 0.0f;
			limit = 1 / dot;
			if (limit < 0) { limit *= -1.0f; }

			//ぶつかっていれば押し返すベクトルを返す.
			if (distance < limit) {
				out.mForceOut = WallVec * ((limit - distance) * dot);
				return true;
			}
		}

		return false;
	}
}