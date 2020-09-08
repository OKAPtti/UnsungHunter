/**
* @file ArsRayInfo.h
* @brief レイの情報を格納した構造体
*/

#pragma once

namespace Alrescha {

	class GameObject;
	//レイ判定が当たった情報を格納する.
	struct RayInfo
	{
		RayInfo(void)
			: mpGameObject(nullptr)
			, mDistance(0.0f)
			, mLimit(0.0f)
			, mDot(0.0f)
			, mRayDir({0.0f,0.0f,0.0f})
			, mRayPos({0.0f,0.0f,0.0f})
			, mNormal({0.0f,0.0f,0.0f})
		{}

		GameObject* mpGameObject;
		//メッシュまでの距離.
		float mDistance;
		float mLimit;
		float mDot;
		D3DXVECTOR3 mRayDir;
		D3DXVECTOR3 mRayPos;
		//レイが当たった座標.
		D3DXVECTOR3 mPosition;
		//法線.
		D3DXVECTOR3 mNormal;

	};
}