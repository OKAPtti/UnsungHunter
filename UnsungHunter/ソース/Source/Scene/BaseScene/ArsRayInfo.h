/**
* @file ArsRayInfo.h
* @brief ���C�̏����i�[�����\����
*/

#pragma once

namespace Alrescha {

	class GameObject;
	//���C���肪�������������i�[����.
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
		//���b�V���܂ł̋���.
		float mDistance;
		float mLimit;
		float mDot;
		D3DXVECTOR3 mRayDir;
		D3DXVECTOR3 mRayPos;
		//���C�������������W.
		D3DXVECTOR3 mPosition;
		//�@��.
		D3DXVECTOR3 mNormal;

	};
}