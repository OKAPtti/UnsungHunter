/**
* @file ArsScene.h
* @brief シーンの基底クラス
*/

#pragma once

#include "../../Core/ArsSingleton.h"
#include "ArsRayInfo.h"
#include "../ArsSceneType.h"

namespace Alrescha {
	class GameFrame;
	class GameObject;
	class CameraSystem;
	class Scene
	{
	public:
		Scene(GameFrame* apGameFrame);
		virtual ~Scene(void);
		
		/**
		* @brief シーンの初期化をします
		* @param Jsonファイル名
		*/
		virtual void InitializeScene(const std::string& aJsonFileName);

		/**
		* @brief 更新をします
		*/
		virtual void Update(void);
		/**
		* @brief 描画をします
		*/
		virtual void Draw(void);

		/**
		* @brief シーンタイプを返します
		*/
		const SceneType& GetType(void)const { return mType; }

		/**
		* @brief シーンにGameObjectを追加します
		* @param 追加したいGameObject
		* @param 描画順
		* @param aDrawOrderは0～9の間で、数字が大きい程後に描画される
		*/
		void AddGameObject(SPtr<GameObject> apGameObject, UINT aDrawOrder = 4U);

		/**
		* @brief 指定した同じタイプのGameObjectリストを返します
		* @param 取得したいタイプ
		* @note ピンポイントで取得したいとき
		*/
		std::list<SPtr<GameObject>> FindGameObjectList(int aType);
		/**
		* @brief GameObjectを返します
		* @param 取得したいタイプ
		* @note ピンポイントで取得したいとき
		*/
		SPtr<GameObject> FindGameObject(int aType);
		/**
		* @brief 指定した範囲のGameObjectリストを返します
		* @param 取得したい初めのタイプ
		* @param 取得したい終わりのタイプ
		* @note ある範囲内で取得したいとき.
		*/
		std::list<SPtr<GameObject>> FindGameObjectList(int aBeginType, int aEndType);

		/**
		* @brief メッシュとの当たり判定を返します
		* @param メッシュ情報
		* @param 行列
		* @param レイの発射位置
		* @param レイの発射方向
		* @param 当たったレイの距離
		* @note 当っているならtrueを返す
		*/
		BOOL Intersect(LPD3DXMESH& aMesh, const D3DXMATRIX& aAnotherMatrix
			, const D3DXVECTOR3& aRayShotPos, const D3DXVECTOR3& aRayDirection,
			float* aDistance);

		/**
		* @brief 当たったリストを返します
		* @note 距離が近い順にソートされます
		*/
		std::list<RayInfo> CameraRayTestList(void);

		/**
		* @brief RayInfoを返します
		* @param レイの発射位置
		* @param レイの発射方向
		* @param 判定したいタイプ
		*/
		RayInfo RayTest(const D3DXVECTOR3& aPosition,
			const D3DXVECTOR3& aRayDirection, int aJudgeType);

		/**
		* @brief デルタタイムを返します
		*/
		float GetDeltaTime(void)const;

		/**
		* @brief カメラを設定します
		*/
		void SetTargetCamera(const SPtr<GameObject>& aTargetObject) { mTargetCamera = aTargetObject; }

		/**
		* @brief カメラを返します
		*/
		CameraSystem* GetCamera(void) { return mpCamera.get(); }

	private:
		GameFrame* mpGameFrame;
		WPtr<GameObject> mTargetCamera;
		UPtr<CameraSystem> mpCamera;

	protected:
		std::vector<std::list<SPtr<GameObject>>> mGameObjectList;

		//今どのシーンかを判断する.
		SceneType mType;

		/**
		* @brief シーンタイプを設定します
		*/
		void SetType(const SceneType& aType) { mType = aType; }
	};
}