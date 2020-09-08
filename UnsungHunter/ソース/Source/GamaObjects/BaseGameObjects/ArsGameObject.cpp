#include "ArsGameObject.h"
#include "../../Core/Model/ArsModelManager.h"

namespace Alrescha {
	GameObject::GameObject(int aType)
		: mpScene(nullptr)
		, mType(aType)
		, mpModel(nullptr)
		, mpTexture(nullptr)
		, mIsAlive(true)
		, mIsVisible(true)
		, mpParent()
	{
		D3DXMatrixIdentity(&mScaleMatrix);
		D3DXMatrixIdentity(&mRotateMatrix);
		D3DXMatrixIdentity(&mTransMatrix);
		D3DXMatrixIdentity(&mMatrix);
	}
	void GameObject::Deserialize(const json11::Json& aJsonObject)
	{
		//モデル読み込み.
		mpModel = GetModelMgr().GetModel(aJsonObject["ModelFileName"].string_value());

		mMatrix = mScaleMatrix * mRotateMatrix * mTransMatrix;
	}
	void GameObject::Initialize(void)
	{
	}
	void GameObject::SetPosition(const D3DXVECTOR3& aPosition)
	{
		mTransMatrix._41 = aPosition.x;
		mTransMatrix._42 = aPosition.y;
		mTransMatrix._43 = aPosition.z;
	}
	void GameObject::SetScale(const D3DXVECTOR3& aScale)
	{
		D3DXMatrixScaling(&mScaleMatrix, aScale.x, aScale.y, aScale.z);
	}
	void GameObject::AddPosition(const D3DXVECTOR3& aAddPosition, bool aIsLocal)
	{
		if (aIsLocal) {
			D3DXMATRIX trans;
			D3DXMatrixIdentity(&trans);
			D3DXMatrixTranslation(&trans, aAddPosition.x, aAddPosition.y, aAddPosition.z);
			mTransMatrix = trans * mTransMatrix;
		}
		else {
			mTransMatrix._41 += aAddPosition.x;
			mTransMatrix._42 += aAddPosition.y;
			mTransMatrix._43 += aAddPosition.z;
		}
	}
	void GameObject::AddRotation(const D3DXMATRIX& aAddRotation, bool aIsLocal)
	{
		if (aIsLocal) {
			mRotateMatrix = aAddRotation * mRotateMatrix;
		}
		else {
			mRotateMatrix = mRotateMatrix * aAddRotation;
		}
	}

	void GameObject::SetRotation(const D3DXMATRIX& aRotationMatrix)
	{
		mRotateMatrix = aRotationMatrix;
	}
	void GameObject::SetRotation(const D3DXVECTOR3& aRadian)
	{
		D3DXMATRIX rotate, tmp;
		D3DXMatrixIdentity(&rotate);
		D3DXMatrixIdentity(&tmp);
		D3DXMatrixRotationX(&tmp, D3DXToRadian(aRadian.x));
		rotate = tmp;
		D3DXMatrixRotationY(&tmp, D3DXToRadian(aRadian.y));
		rotate *= tmp;
		D3DXMatrixRotationZ(&tmp, D3DXToRadian(aRadian.z));
		rotate *= tmp;

		mRotateMatrix = rotate;
	}
	void GameObject::Draw3D(void)
	{
		if (!mpModel) { return; }
		Get3D().SetWorldMatrix(&mMatrix);
		mpModel->Draw();
	}
	bool GameObject::HitCheckSphere(const InputSphere& in, OutputSphere& out)
	{
		//２点間の距離を求める.
		D3DXVECTOR3 pos = GetPosition();//自分の場所(当てられる方).
		auto diff = pos - in.mCenterPos;//二点の差分を計算.
		out.mDistance = D3DXVec3Length(&diff);//距離を求める.

		//当たったかどうか.
		if (out.mDistance <= in.mRadius * 2) { out.mIsHit = true; }

		//２点間の中央を求める.
		out.mHitPos = in.mCenterPos + (diff * 0.5f);

		return out.mIsHit;
	}

	bool GameObject::HitCheckCircle(const InputCircle& in, OutputCircle& out)
	{
		//２点間の距離
		D3DXVECTOR3 pos = GetPosition();
		pos.y = 0.0f;
		D3DXVECTOR3 vec = { in.mCenterPos - pos };
		out.mLength = D3DXVec3Length(&vec);
		//求めた距離が衝突距離より小さいか.
		if (out.mLength < in.mCollisionLength) {
			vec *= (1.0f - (out.mLength / in.mCollisionLength));
			out.mForceOut = vec;
			out.mIsHit = true;
		}

		return out.mIsHit;
	}
	bool GameObject::HitCheckRay(const InputRay& in, OutputRay& out)
	{

		if (!mpModel) {
			out.mIsHit = false;
			out.mDistance = 0.0f;

			return out.mIsHit;
		}

		D3DXMATRIX invMat;
		D3DXMatrixIdentity(&invMat);
		D3DXMatrixInverse(&invMat, nullptr, &GetMatrixConst());

		D3DXVECTOR3 localPos, localVec;
		D3DXVec3TransformCoord(&localPos, &in.mRayPos, &invMat);

		D3DXVec3TransformNormal(&localVec, &in.mRayDir, &invMat);

		BOOL hit;
		float distance;

		D3DXIntersect(mpModel->GetMesh(), &localPos, &localVec, &hit, nullptr, nullptr, nullptr, &distance, nullptr, nullptr);

		out.mIsHit = hit;
		out.mDistance = distance;

		return out.mIsHit;
	}
}