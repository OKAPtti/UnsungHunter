#include "ArsQuiver.h"
#include "../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	Quiver::Quiver(void)
		: GameObject(eQuiver)
	{}
	Quiver::~Quiver(void)
	{
	}
	void Quiver::Initialize(void)
	{
		GameObject::Initialize();
		mpModel = GetModelMgr().GetModel("Quiver_01");
	}
	void Quiver::Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		D3DXMATRIX trans,rotate;
		D3DXMatrixIdentity(&trans);
		D3DXMatrixIdentity(&rotate);

		//e‚©‚ç‚Ç‚ê‚­‚ç‚¢—£‚·‚©A‰ñ“]‚·‚é‚©.
		D3DXMatrixTranslation(&trans, 0.0625f, 0.25f, -0.174f);
		D3DXMatrixRotationZ(&rotate, D3DXToRadian(-25.0f));
		mMatrix = rotate * trans * aParentMatrix;
	}
	void Quiver::Draw3D(void)
	{
		Get3D().SetWorldMatrix(&mMatrix);
		mpModel->Draw();
	}
}
