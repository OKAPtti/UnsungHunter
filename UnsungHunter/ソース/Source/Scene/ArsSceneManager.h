/**
* @file ArsSceneManager.h
* @brief シーンを管理するクラス
*/

#pragma once

#include "ArsSceneType.h"
#include "../Core/ArsSingleton.h"

namespace Alrescha {
	class Scene;
	class GameFrame;
	class Camera;
	class SceneManager :public Singleton<SceneManager>
	{
	public:
		friend class Singleton<SceneManager>;

		/**
		* @brief 初期化をします
		* @param GameFrame
		*/
		void Initialize(GameFrame* apGameFrame);
		
		/**
		* @brief 終了処理をします
		*/
		void Finalize(void);

		/**
		* @brief シーンを切り替えます
		* @param 切り替えたいシーンタイプ
		*/
		void ChangeScene(const SceneType& aType);
		/**
		* @brief 更新をします
		*/
		void Update(void);
		/**
		* @brief 描画をします
		*/
		void Draw(void);

		/**
		* @brief シーンを返します
		*/
		Scene* GetScene(void);

	private:
		UPtr<Scene> mpScene;
		GameFrame* mpGameFrame;

	protected:
		SceneManager(void);
		~SceneManager(void);
	};
	inline SceneManager& GetSceneMgr(void) { return SceneManager::GetInstance(); }
}