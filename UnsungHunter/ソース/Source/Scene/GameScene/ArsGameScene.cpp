#include "ArsGameScene.h"
#include "../ArsSceneType.h"
#include "../../Core/GameFrame/ArsGameFrame.h"
#include "../../Core/Input/ArsInput.h"

#include "../../GamaObjects/ArsAllGameObjectIncludes.h"
#include "../../GamaObjects/ArsGameObjectType.h"
#include "../ArsSceneManager.h"

#include "../ResultScene/ArsResult.h"
#include "../../GamaObjects/ArsAllGameObjectIncludes.h"
#include "../../Core/Camera/ArsCameraSystem.h"

namespace Alrescha {

	GameScene::GameScene(GameFrame* apGameFrame)
		: Scene(apGameFrame)
		, mIsResultCreated(false)
	{
		Scene::SetType(SceneType::eGameScene);

		ShowCursor(FALSE);

		{//プレイヤー.
			auto pUI = std::make_shared<UserInterface>();
			Scene::AddGameObject(pUI, 0U);
			pUI->Initialize();

			auto pPlayer = std::make_shared<Player>();
			Scene::AddGameObject(pPlayer, 1U);
			pPlayer->Initialize();

			CameraSystem* pCamera = GetCamera();
			pCamera->SetTargetViewpoint(pPlayer);
			Scene::SetTargetCamera(pPlayer);
		}

		{//ステージにある物を生成.
			Scene::InitializeScene("../実行/Data/Json/GameScene.json");
		}

		{//敵の生成.
			const auto Json = LoadJson("../実行/Data/Json/GameScene_EnemyList.json");
			const auto GameObjectList = Json["EnemyList"].array_items();

			//小数点付き数値はdouble型なので、float型にキャストするために一時格納.
			json11::Json::array jsonAry;

			for (const auto& it : GameObjectList) {
				SPtr<Enemy> pEnemy = nullptr;
				//敵の名前によって生成するクラスを変更.
				if (it["ClassName"].string_value() == "Wolf") {
					pEnemy = std::make_shared<Wolf>();
				}
				else if (it["ClassName"].string_value() == "Boss") {
					pEnemy = std::make_shared<Boss>();
				}

				AddGameObject(pEnemy, 4U);

				pEnemy->Initialize();
				jsonAry = it["Position"].array_items();
				//座標.
				if (!jsonAry.empty()) {
					pEnemy->SetPosition({
							CastFloat(jsonAry[0].number_value()),
							CastFloat(jsonAry[1].number_value()),
							CastFloat(jsonAry[2].number_value()),
						});
				}

				//回転.
				jsonAry = it["Rotation"].array_items();
				if (!jsonAry.empty()) {
					pEnemy->SetRotation({
							CastFloat(jsonAry[0].number_value()),
							CastFloat(jsonAry[1].number_value()),
							CastFloat(jsonAry[2].number_value()),
						});
				}
			}

		}
	}

	GameScene::~GameScene(void)
	{
		ShowCursor(TRUE);
	}

	void GameScene::Update(void)
	{
		Scene::Update();

		//ESC押されたらタイトルシーンに遷移.
		if (GetInput().GetKeyDown(VK_ESCAPE)) {
			GetSceneMgr().ChangeScene(SceneType::eTitleScene);
		}

		if (mIsResultCreated) {
			if (GetInput().GetKeyDown(VK_LBUTTON)) {
				GetSceneMgr().ChangeScene(SceneType::eTitleScene);
			}
		}

		auto&& player = Scene::FindGameObject(ePlayer);
		if (!player) { return; }
		auto pPlayer = player->Cast<Player>(ePlayer);
		auto&& boss = Scene::FindGameObject(eBoss);
		if (!boss) { return; }
		auto pBoss = boss->Cast<Boss>(eBoss);
		if (!pBoss) { return; }
		//プレイヤーが死亡していたらGameOver.
		if (pPlayer->GetHp()->IsEmpty()) {
			ResultInit(GameObjectType::ePlayer);
			if (GetInput().GetKeyDown(VK_LBUTTON)) {
				GetSceneMgr().ChangeScene(SceneType::eTitleScene);
			}
		}
		else if (pBoss->GetHp()->IsEmpty()) {
			ResultInit(GameObjectType::eBoss);
			if (GetInput().GetKeyDown(VK_LBUTTON)) {
				GetSceneMgr().ChangeScene(SceneType::eTitleScene);
			}
		}


	}

	void GameScene::Release(void)
	{}

	void GameScene::ResultInit(const GameObjectType& aKilledCharacter)
	{
		if (mIsResultCreated) { return; }

		auto pResult = std::make_shared<Result>();

		pResult->Initialize(aKilledCharacter);

		Scene::AddGameObject(pResult, 1U);
		mIsResultCreated = true;
	}

	void GameScene::Draw()
	{
		Scene::Draw();
	}
}