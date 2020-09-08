#include "ArsBow.h"
#include "Arrow/ArsAllArrowIncludes.h"
#include "../../Scene/BaseScene/ArsScene.h"
#include "../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	Bow::Bow(void)
		: Weapon(eBow)
		, mShiftMatrix()
	{
		D3DXMatrixIdentity(&mShiftMatrix);
	}
	Bow::~Bow(void)
	{}
	void Bow::Initialize(void)
	{
		Weapon::Initialize();

		mpModel = GetModelMgr().GetModel("Bow_01");

		D3DXMATRIX tmpMat;
		D3DXMatrixIdentity(&tmpMat);
		D3DXMatrixIdentity(&mShiftMatrix);
		D3DXMatrixRotationY(&tmpMat, D3DXToRadian(240.0f));
		mShiftMatrix *= tmpMat;
		D3DXMatrixRotationX(&tmpMat, D3DXToRadian(90.0f));
		mShiftMatrix *= tmpMat;
	}
	void Bow::Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		//�e����ǂꂭ�炢������.
		D3DXMATRIX tmpMat;
		D3DXMatrixTranslation(&tmpMat, -0.15f, -0.575f, 0.0f);
		mTransMatrix = tmpMat * aParentMatrix;

		mMatrix = mShiftMatrix * mTransMatrix;
	}
	void Bow::Draw3D(void)
	{
		Get3D().SetWorldMatrix(&mMatrix);
		mpModel->Draw();
	}
	void Bow::Shot(const ArrowType& aArrowType)
	{
		//�n���ꂽ�^�C�v�ɂ���Đ�������N���X��ς���.
		SPtr<BaseArrow> arrow;
		switch (aArrowType)
		{
		case ArrowType::eNormalArrow:
			arrow = std::make_shared<NormalArrow>();
			break;
		case ArrowType::eFireArrow:
			arrow = std::make_shared<FireArrow>();
			break;
		default:
			_ASSERT_EXPR(false, L"���݂��Ȃ�������ɓn����Ă��܂��B");
			break;
		}
		//��̑��x.
		float power = 1.0f;

		//���C����ň�Ԏ�O�œ����������̏����擾.
		const auto infoList = mpScene->CameraRayTestList();
		if (!infoList.empty()) {
			//��������Ώە��ւ̃x�N�g��.
			const auto ShootAngle = infoList.front().mPosition - GetPosition();

			//��������.
			arrow->SetScene(mpScene);
			arrow->Initialize(ShootAngle, power, {mMatrix._41,mMatrix._42, mMatrix._43});
			mpScene->AddGameObject(arrow, 5U);
		}
	}
}