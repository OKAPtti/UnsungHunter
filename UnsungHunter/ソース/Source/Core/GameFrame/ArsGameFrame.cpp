#include "ArsGameFrame.h"
#include "../ArsIncludeCores.h"
#include "../../Scene/ArsSceneManager.h"
#include "../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {
	GameFrame::GameFrame(void)
		: mpHwnd(nullptr)
		, mpFpsCounter(nullptr)
		, mpFpsAdjuster(nullptr)
		, mCanFinishGame(true)
	{
		//メモリリーク検出.
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	GameFrame::~GameFrame(void)
	{
		GetTexMgr().Finalize();
		GetModelMgr().Finalize();
		GetSoundManager().Finalize();
	}

	void GameFrame::Initialize(HWND apHwnd)
	{
		mpFpsCounter = std::make_unique<FpsCounter>();
		constexpr int MaxFps = 60;
		mpFpsAdjuster = std::make_unique<FpsAdjuster>(MaxFps);

		mpHwnd = apHwnd;
		GetInput().Initialize(mpHwnd);
		GetSoundManager().Initialize(mpHwnd);

		auto pD3DDevice = Get3D().GetDevice();

		GetTexMgr().Initialize(pD3DDevice);
		GetModelMgr().Initialize(pD3DDevice);

		GetSceneMgr().Initialize(this);
	}

	bool GameFrame::Run(void)
	{
		mpFpsCounter->End();
		mpFpsCounter->Start();
		auto pD3DDevice = Get3D().GetDevice();

		// バックバッファと Z バッファをクリア
		pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(62, 58, 57), 1.0f, 0);

		Update();

		// 描画開始
		pD3DDevice->BeginScene();

		//描画.
		GetSceneMgr().Draw();

		// 描画終了
		pD3DDevice->EndScene();

		mpFpsAdjuster->Wait();

		// バックバッファをプライマリバッファにコピー
		pD3DDevice->Present(NULL, NULL, NULL, NULL);

		//ゲーム終了できて、ESCキーを押されていればゲーム終了.
		if (mCanFinishGame && GetInput().GetKeyDown(VK_ESCAPE)) { return false; }

		return true;
	}

	void GameFrame::Update(void)
	{
		//入力デバイスの更新.
		GetInput().Update();

		//シーンの更新.
		GetSceneMgr().Update();

		//ループ再生させる音楽の更新.
		GetSoundManager().Update();
	}
	float GameFrame::GetDeltaTime(void) const
	{
		return mpFpsCounter->GetDeltaTime();
	}
}