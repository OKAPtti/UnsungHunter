#include "ArsNormalArrow.h"
#include "../../Effect/ArsEffect.h"

namespace Alrescha {
	NormalArrow::NormalArrow(void)
		: BaseArrow(eNormalArrow)
	{}
	NormalArrow::~NormalArrow(void)
	{}
	void NormalArrow::Initialize(const D3DXVECTOR3& aShootAngle, float aPower, const D3DXVECTOR3& aShotPosition)
	{
		BaseArrow::Initialize(aShootAngle, aPower, aShotPosition);
		//�_���[�W�ʃZ�b�g.
		mDamage = 34;

		mpEffect->SetVertexColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	void NormalArrow::Update(float aDeltaTime)
	{
		BaseArrow::Update(aDeltaTime);
	}
	void NormalArrow::Draw3D(void)
	{
		BaseArrow::Draw3D();
	}
	void NormalArrow::OnCollision(void)
	{
		BaseArrow::OnCollision();
	}
}