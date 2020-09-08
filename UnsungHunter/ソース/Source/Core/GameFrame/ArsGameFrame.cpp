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
		//���������[�N���o.
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

		// �o�b�N�o�b�t�@�� Z �o�b�t�@���N���A
		pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(62, 58, 57), 1.0f, 0);

		Update();

		// �`��J�n
		pD3DDevice->BeginScene();

		//�`��.
		GetSceneMgr().Draw();

		// �`��I��
		pD3DDevice->EndScene();

		mpFpsAdjuster->Wait();

		// �o�b�N�o�b�t�@���v���C�}���o�b�t�@�ɃR�s�[
		pD3DDevice->Present(NULL, NULL, NULL, NULL);

		//�Q�[���I���ł��āAESC�L�[��������Ă���΃Q�[���I��.
		if (mCanFinishGame && GetInput().GetKeyDown(VK_ESCAPE)) { return false; }

		return true;
	}

	void GameFrame::Update(void)
	{
		//���̓f�o�C�X�̍X�V.
		GetInput().Update();

		//�V�[���̍X�V.
		GetSceneMgr().Update();

		//���[�v�Đ������鉹�y�̍X�V.
		GetSoundManager().Update();
	}
	float GameFrame::GetDeltaTime(void) const
	{
		return mpFpsCounter->GetDeltaTime();
	}
}