#include "ArsTitleScene.h"
#include "../ArsSceneType.h"
#include "../../Core/GameFrame/ArsGameFrame.h"
#include "../../Core/Input/ArsInput.h"
#include "ArsTitle.h"
#include "../ArsSceneManager.h"
#include "../../GamaObjects/UserInterface/ArsUserInterface.h"

namespace Alrescha {

	TitleScene::TitleScene(GameFrame* apGameFrame)
		: Scene(apGameFrame)
	{
		Scene::SetType(SceneType::eTitleScene);

		auto pTitle = std::make_shared<Title>();
		pTitle->Initialize();
		Scene::AddGameObject(pTitle, 0U);
	}


	TitleScene::~TitleScene(void)
	{
	}

	void TitleScene::Update(void)
	{
		Scene::Update();

		//左クリックされたゲームシーンに遷移.
		if (GetInput().GetKeyDown(VK_LBUTTON)) {
			GetSceneMgr().ChangeScene(SceneType::eGameScene);
		}
	}

	void TitleScene::Draw()
	{
		Scene::Draw();
	}
}