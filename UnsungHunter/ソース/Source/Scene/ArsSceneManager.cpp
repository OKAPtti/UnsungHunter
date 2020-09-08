#include "ArsSceneManager.h"
#include "ArsAllSceneIncludes.h"
#include "ArsSceneType.h"
#include "../Core/GameFrame/ArsGameFrame.h"

namespace Alrescha {
	void SceneManager::Initialize(GameFrame*apGameFrame)
	{
		mpGameFrame = apGameFrame;
		mpScene = std::make_unique<TitleScene>(apGameFrame);
	}

	void SceneManager::Finalize(void)
	{
	}

	void SceneManager::ChangeScene(const SceneType& aType)
	{
		switch (aType)
		{
		case SceneType::eTitleScene:
			mpScene = std::make_unique<TitleScene>(mpGameFrame);
			break;
		case SceneType::eGameScene:
			mpScene = std::make_unique<GameScene>(mpGameFrame);
			break;
		default:
			_ASSERT_EXPR(false, L"‘¶Ý‚µ‚È‚¢Scene‚ð“n‚³‚ê‚Ü‚µ‚½B");
			break;
		}
	}

	void SceneManager::Update(void)
	{
		if (!mpScene) { return; }
		mpScene->Update();
	}

	void SceneManager::Draw(void)
	{
		if (!mpScene) { return; }
		mpScene->Draw();
	}

	Scene* SceneManager::GetScene(void)
	{
		return mpScene.get();
	}

	SceneManager::SceneManager(void)
		: mpScene(nullptr)
		, mpGameFrame(nullptr)
	{}
	SceneManager::~SceneManager(void)
	{}
}