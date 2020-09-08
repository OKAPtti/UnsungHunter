/**
* @file ArsSceneManager.h
* @brief �V�[�����Ǘ�����N���X
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
		* @brief �����������܂�
		* @param GameFrame
		*/
		void Initialize(GameFrame* apGameFrame);
		
		/**
		* @brief �I�����������܂�
		*/
		void Finalize(void);

		/**
		* @brief �V�[����؂�ւ��܂�
		* @param �؂�ւ������V�[���^�C�v
		*/
		void ChangeScene(const SceneType& aType);
		/**
		* @brief �X�V�����܂�
		*/
		void Update(void);
		/**
		* @brief �`������܂�
		*/
		void Draw(void);

		/**
		* @brief �V�[����Ԃ��܂�
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