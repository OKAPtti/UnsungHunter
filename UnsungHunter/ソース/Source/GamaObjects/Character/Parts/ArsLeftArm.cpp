#include "ArsLeftArm.h"
#include "../ArsPlayer.h"

namespace Alrescha {
	LeftArm::LeftArm(void)
		: ChildGameObject(eLeftArm)
	{}
	LeftArm::~LeftArm(void)
	{}
	void LeftArm::Initialize(const std::string& aModelName)
	{
		ChildGameObject::Initialize(aModelName);
	}
	void LeftArm::UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		auto parent = mpParent.lock();
		if (!parent) { return; }

		D3DXMATRIX parentMat;
		D3DXMatrixIdentity(&parentMat);

		mTransMatrix = parent->GetTransMatrix();
		parentMat = parent->GetRotateMatrix();

		//e‚©‚ç‚Ç‚ê‚­‚ç‚¢—£‚·‚©.
		D3DXVECTOR3 vec(-0.17f, 0.2f, 0.0f);
		D3DXVec3TransformCoord(&vec, &vec, &parentMat);

		mRotateMatrix = parentMat;

		D3DXMATRIX tmpMat;
		D3DXMatrixTranslation(&tmpMat, vec.x, vec.y, vec.z);
		mTransMatrix = tmpMat * mTransMatrix;

		auto player = parent->Cast<Player>(ePlayer);
		if (!player) { return; }
		//“®‚¢‚Ä‚¢‚ÄA‚©‚ÂA‹|‚ð\‚¦‚Ä‚¢‚È‚¯‚ê‚Î.
		if (player->GetIsMove() && !player->GetIsBendBow()) {
			WalkAnimation();
		}
		else if (player->GetIsBendBow()) {
			HoldingBow();
			D3DXMATRIX rotate;
			D3DXMatrixRotationZ(&rotate, D3DXToRadian(-60.0f));
			mRotateMatrix = rotate * mRotateMatrix;
		}

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
	}

	void LeftArm::Draw(float aAlphaValue)
	{
		ChildGameObject::Draw(aAlphaValue);
	}
	void LeftArm::WalkAnimation(void)
	{
		D3DXQUATERNION StartQua, EndQua, NowQua;

		D3DXMATRIX frontMat, backMat;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(-40.0f));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(40.0f));

		D3DXQuaternionRotationMatrix(&StartQua, &frontMat);
		D3DXQuaternionRotationMatrix(&EndQua, &backMat);

		D3DXQuaternionSlerp(&NowQua, &StartQua, &EndQua, mAnimeFrame);


		constexpr float frameSize = 0.0475f;
		static bool IsReverse = false;

		if (!IsReverse) {
			mAnimeFrame += frameSize;
		}
		else {
			mAnimeFrame -= frameSize;
		}

		if (mAnimeFrame > 1.0f) {
			IsReverse = true;
		}
		if (mAnimeFrame < 0.0f) {
			IsReverse = false;
		}

		D3DXMATRIX nowMat;
		D3DXMatrixRotationQuaternion(&nowMat, &NowQua);

		mRotateMatrix = nowMat * mRotateMatrix;
	}
	void LeftArm::HoldingBow(void)
	{
		D3DXQUATERNION StartQua, EndQua, NowQua;

		D3DXMATRIX frontMat, backMat;

		D3DXMatrixRotationX(&frontMat, D3DXToRadian(-90.0f));
		D3DXMatrixRotationX(&backMat, D3DXToRadian(-90.0f));

		D3DXQuaternionRotationMatrix(&StartQua, &frontMat);
		D3DXQuaternionRotationMatrix(&EndQua, &backMat);

		//•à‚¢‚Ä‚¢‚éó‘Ô‚©‚çˆá˜aŠ´‚ð–³‚­‚·‚½‚ß.
		static float animeFrame = mAnimeFrame;
		D3DXQuaternionSlerp(&NowQua, &StartQua, &EndQua, animeFrame);


		constexpr float frameSize = 0.035f;
		static bool IsReverse = false;

		animeFrame += frameSize;
		if (animeFrame > 1.0f) {
			animeFrame = 1.0f;
		}

		D3DXMATRIX nowMat;
		D3DXMatrixRotationQuaternion(&nowMat, &NowQua);

		mRotateMatrix = nowMat * mRotateMatrix;
	}
}