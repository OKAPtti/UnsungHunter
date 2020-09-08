#include "ArsHead.h"
#include "../ArsPlayer.h"
#include "../../../Core/DirectX/ArsDirectGraphics.h"

namespace Alrescha {
	Head::Head(void)
		: ChildGameObject(eHead)
	{}
	Head::~Head(void)
	{}
	void Head::Initialize(const std::string& aModelName)
	{
		ChildGameObject::Initialize(aModelName);
	}
	void Head::UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		D3DXMATRIX parentMat;
		auto parent = mpParent.lock();
		if (parent != nullptr) {
			mTransMatrix = parent->GetTransMatrix();
			parentMat = parent->GetRotateMatrix();
		}
		
		//e‚©‚ç‚Ç‚ê‚­‚ç‚¢—£‚·‚©.
		D3DXVECTOR3 vec(0.0f, 0.275f, 0.0f);
		D3DXVec3TransformCoord(&vec, &vec, &parentMat);

		mRotateMatrix = parentMat;

		D3DXMATRIX tmpMat;
		D3DXMatrixTranslation(&tmpMat, vec.x, vec.y, vec.z);
		mTransMatrix = tmpMat * mTransMatrix;

		auto player = parent->Cast<Player>(ePlayer);
		if (!player) { return; }
		if (player->GetIsBendBow()) {
			D3DXMATRIX rotate;
			D3DXMatrixRotationY(&rotate, D3DXToRadian(-30.0f));
			mRotateMatrix = rotate * mRotateMatrix;
		}

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
	}
	void Head::Draw(float aAlphaValue)
	{
		ChildGameObject::Draw(aAlphaValue);
	}
	void Head::DrawRed(void)
	{
		ChildGameObject::DrawRed();
	}
}