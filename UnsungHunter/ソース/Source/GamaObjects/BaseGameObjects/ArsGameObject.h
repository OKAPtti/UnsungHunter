/**
* @file ArsGameObject.h
* @brief ゲームに登場するオブジェクトに必要なものをまとめたクラス
*/

#pragma once

#include "../ArsGameObjectType.h"

namespace Alrescha {
	class Scene;
	class InputSphere;
	class OutputSphere;
	class InputCircle;
	class OutputCircle;
	class InputRayPoly;
	class OutputRayPoly;
	class InputRay;
	class OutputRay;
	class Texture;
	class Model;
	class GameObject :public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(int aType);
		virtual ~GameObject(void) {}


		/**
		* @brief デシリアライズ
		* @param Jsonオブジェクト
		*/
		virtual void Deserialize(const json11::Json& aJsonObject);
		/**
		* @brief 初期化をします
		*/
		virtual void Initialize(void);
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		virtual void Update(float aDeltaTime) {}
		/**
		* @brief 2D描画をします
		*/
		virtual void Draw2D(void) {}
		/**
		* @brief 3D描画をします
		*/
		virtual void Draw3D(void);

		//当たり判定.
		/**
		* @brief 球判定をします
		* @param 当たり判定に使用する情報
		* @param 当たり判定の結果を返す
		*/
		bool HitCheckSphere(const InputSphere& in, OutputSphere& out);
		/**
		* @brief 円判定をします
		* @param 当たり判定に使用する情報
		* @param 当たり判定の結果を返す
		*/
		bool HitCheckCircle(const InputCircle& in, OutputCircle& out);
		/**
		* @brief レイ判定をします
		* @param 当たり判定に使用する情報
		* @param 当たり判定の結果を返す
		*/
		virtual bool HitCheckRay(const InputRay& in, OutputRay& out);
		/**
		* @brief 板ポリゴンレイ判定をします
		* @param 当たり判定に使用する情報
		* @param 当たり判定の結果を返す
		*/
		virtual bool HitCheckRayPoly(const InputRayPoly& in, OutputRayPoly& out) { return false; }

		//行列取得.
		/**
		* @brief SRTが合成された行列を返します
		* @note ※参照返しなので不必要に呼ばないこと
		*/
		D3DXMATRIX& GetMatrix(void)						{ return mMatrix; }
		/**
		* @brief SRTが合成された行列を返します
		*/
		const D3DXMATRIX& GetMatrixConst(void)const		{ return mMatrix; }
		/**
		* @brief 移動行列を返します
		*/
		const D3DXMATRIX& GetTransMatrix(void)const		{ return mTransMatrix; }
		/**
		* @brief 回転行列を返します
		*/
		const D3DXMATRIX& GetRotateMatrix(void)const	{ return mRotateMatrix; }
		
		
		/**
		* @brief 行列を設定します
		* @param 設定する行列
		*/
		void SetMatrix(const D3DXMATRIX& aMatrix)		{ mMatrix = aMatrix; }

		/**
		* @brief 座標(位置)を返します
		*/
		D3DXVECTOR3 GetPosition(void)const				{return D3DXVECTOR3{ mTransMatrix._41,mTransMatrix._42,mTransMatrix._43 };}

		/**
		* @brief 座標(位置)を設定します
		* @param 位置情報が格納されている3Dベクトル
		*/
		void SetPosition(const D3DXVECTOR3& aPosition);
		/**
		* @brief 回転を設定します
		* @param 回転情報が設定されている行列
		*/
		void SetRotation(const D3DXMATRIX& aRotationMatrix);
		/**
		* @brief 回転を設定します
		* @param 回転情報が格納されている3Dベクトル
		*/
		void SetRotation(const D3DXVECTOR3& aRadian);
		/**
		* @brief 拡縮を設定します
		* @param 拡縮情報が設定されている3Dベクトル
		*/
		void SetScale(const D3DXVECTOR3& aScale);

		/**
		* @brief 座標(位置)を加算
		* @param 加算する3Dベクトル
		* @param ローカル座標に加算するかどうか
		* @note 設定されていない場合はローカル座標 
		*/
		void AddPosition(const D3DXVECTOR3& aAddPosition, bool aIsLocal = true);
		/**
		* @brief 回転を加算
		* @param 加算する3Dベクトル
		* @param ローカルに加算するかどうか
		* @note 設定されていない場合はローカル
		*/
		void AddRotation(const D3DXMATRIX& aAddRotation, bool aIsLocal = true);

		/**
		* @brief 描画するかどうか
		* @param 描画するかのフラグ
		* @note true:描画する false:描画しない
		*/
		void SetIsVisible(bool aIsVisible)	{ mIsVisible = aIsVisible; }
		
		/**
		* @brief 描画するかどうかのフラグを返します
		*/
		bool GetIsVisible(void)const		{ return mIsVisible; }

		/**
		* @brief 生存フラグをfalseにします
		* @note Killすると更新も描画もされない
		* @note このGameObjectが、誰からも参照されなくなったらdeleteを行う
		*/
		void Kill(void)				{ mIsAlive = false; }
		/**
		* @brief 生存フラグを返します
		*/
		bool GetIsAlive(void)const	{ return mIsAlive; }

		/**
		* @brief 自身のタイプを返します
		*/
		int GetType(void)const { return mType; }

		/**
		* @brief 親を設定します
		* @param 親にしたいオブジェクト
		*/
		void SetParent(const SPtr<GameObject>& apParent) { mpParent = apParent; }

		/**
		* @brief シーンを設定します
		* @param 設定したいシーン
		*/
		void SetScene(Scene* apScene)	{ if (apScene) mpScene = apScene; }

		/**
		* @brief 自身を(static)キャストします
		* @param タイプ
		* @note このキャストを使うときは引数が決まっている時だけ.
		*/
		template<class T>
		SPtr<T> Cast(int aType);

		/**
		* @brief モデルクラスを返します
		*/
		Model* GetModel(void)const { return mpModel; }
		/**
		* @brief テクスチャクラスを返します
		*/
		Texture* GetTexture(void)const { return mpTexture; }

	private:
		//何のクラスかを判断.
		const int mType;

	protected:
		Scene*				mpScene;

		bool				mIsAlive;
		//描画するかどうか.
		bool				mIsVisible;

		Texture*			mpTexture;
		Model*				mpModel;

		//親子関係.
		WPtr<GameObject>	mpParent;

		//行列.
		D3DXMATRIX			mMatrix;
		D3DXMATRIX			mTransMatrix, mRotateMatrix, mScaleMatrix;

		//衝突判定.
		virtual void OnCollision(void) {}
	};

	//タイプが一意に決まっている場合のみ使用可.
	template<class T>
	inline SPtr<T> GameObject::Cast(int aType)
	{
		//引数で渡されたタイプと同じであればキャスト.
		if (GetType() == aType) {
			return std::static_pointer_cast<T>(shared_from_this());
		}

		//違っていたら.
		return nullptr;
	}

	//以下当たり判定用.
	class InputSphere {
	public:
		D3DXVECTOR3 mCenterPos = { 0.0f,0.0f,0.0f };//当たり判定の中心.
		float mRadius = 0.0f;						//当たり判定の半径.
	};
	class OutputSphere {
	public:
		bool mIsHit = false;						//当たったかどうか.
		float mDistance = 0.0f;						//２点間の距離.
		D3DXVECTOR3 mHitPos = { 0.0f,0.0f,0.0f };	//２点間の中点
	};
	class InputCircle {
	public:
		D3DXVECTOR3 mCenterPos = { 0.0f,0.0f,0.0f };//当たり判定の中心.
		float mCollisionLength = 0.0f;				//当たり判定の半径.
	};
	class OutputCircle {
	public:
		bool mIsHit = false;						//当たったかどうか.
		float mLength = 0.0f;						//２点間の距離.
		D3DXVECTOR3 mHitPos = { 0.0f,0.0f,0.0f };	//２点間の中点
		D3DXVECTOR3 mForceOut = { 0.0f,0.0f,0.0f };	//押し出す距離.
	};
	class InputRayPoly {
	public:
		D3DXVECTOR3 mRayPos = { 0.0f,0.0f,0.0f };	//レイの発射位置.
		D3DXVECTOR3 mRayDir = { 0.0f,0.0f,1.0f };	//レイの発射方向.
	};
	class OutputRayPoly {
	public:
		bool mIsHit = false;
		float mDistance = 0.0f;
	};
	class InputRay {
	public:
		D3DXVECTOR3 mRayPos = { 0.0f,0.0f,0.0f };	//レイの発射位置.
		D3DXVECTOR3 mRayDir = { 0.0f,0.0f,1.0f };	//レイの発射方向.
	};
	class OutputRay {
	public:
		bool mIsHit = false;						//当たったかどうか.
		float mDistance = 0.0f;						//２点間の距離.
		D3DXVECTOR3 mNormal = { 0.0f,0.0f,0.0f };	//当たったポリゴンの法線.
		D3DXVECTOR3 mForceOut = { 0.0f,0.0f,0.0f };	//押し出す距離.
	};
}
