#include "ArsChildGameObject.h"
#include "../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	ChildGameObject::ChildGameObject(int aType)
		: GameObject(aType)
		, mChildList()
		, mAnimeFrame(0.0f)
	{}
	ChildGameObject::~ChildGameObject(void)
	{}
	void ChildGameObject::Initialize(const std::string & aModelName)
	{
		GameObject::Initialize();
		//仮想親は描画しないのでnullptrを代入.
		if (aModelName == "Virtual") { 
			mpModel = nullptr; 
			return;
		}
		mpModel = GetModelMgr().GetModel(aModelName);
	}
	void ChildGameObject::Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime)
	{
		for (auto&& it : mChildList) {
			it->UpdateTransform(aParentMatrix,aDeltaTime);
		}
	}
	void ChildGameObject::WalkAnimation(void)
	{
		for (auto&& it : mChildList) {
			it->WalkAnimation();
		}
	}
	void ChildGameObject::Draw(float aAlphaValue)
	{
		Get3D().SetWorldMatrix(&mMatrix);
		if (mpModel) { 
			mpModel->Draw(aAlphaValue); 
		}

		for (auto&& it : mChildList) {
			it->Draw(aAlphaValue);
		}
	}
	void ChildGameObject::DrawRed(void)
	{
		Get3D().SetWorldMatrix(&mMatrix);
		if (mpModel) {
			mpModel->DrawRed();
		}

		for (auto&& it : mChildList) {
			it->DrawRed();
		}
	}
	void ChildGameObject::AddChild(SPtr<ChildGameObject> apAddChildList)
	{
		mChildList.push_back(apAddChildList);
	}
	void ChildGameObject::AddChild(std::list<SPtr<ChildGameObject>> apAddChildList)
	{
		for (auto&& it : apAddChildList) {
			mChildList.push_back(it);
		}
	}
	void ChildGameObject::GetChildList(std::list<SPtr<ChildGameObject>>& aAnotherList)
	{
		aAnotherList = mChildList;
	}
	void ChildGameObject::GetChild(std::list<SPtr<ChildGameObject>>& aAnotherList, int aType)
	{
		for (auto&& listIt : mChildList) {
			if (listIt->GetType() == aType) { aAnotherList.push_back(listIt); }
		}
	}
	void ChildGameObject::AllAnimeFrameReset(void)
	{
		//アニメーションリセット.
		mAnimeFrame = 0.0f;
		for (auto&& it : mChildList) {
			it->AllAnimeFrameReset();
		}
	}
}