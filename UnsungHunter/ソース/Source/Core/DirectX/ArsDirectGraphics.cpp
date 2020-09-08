#include "ArsDirectGraphics.h"
#include "../ArsConfiguration.h"
#include "ArsSprite.h"

namespace Alrescha {
	DirectGraphics::DirectGraphics(void)
		: mpD3D(nullptr)
		, mpD3DDevice(nullptr)
		, mD3Dpp()
		, mLight()
		, mpSprite(nullptr)
		, mCamera()
	{}
	DirectGraphics::~DirectGraphics(void)
	{}

	void DirectGraphics::Initialize(HWND apHwnd, bool aIsFullScreen)
	{
		//ライトの設定.
		ZeroMemory(&mLight, sizeof(D3DLIGHT9));
		mLight.Type = D3DLIGHT_DIRECTIONAL;	
		//色の設定(0～1).
		//拡散光.
		mLight.Diffuse.r = 1.0f;
		mLight.Diffuse.g = 1.0f;
		mLight.Diffuse.b = 1.0f;
		//環境光.
		constexpr float AmbientValue = 0.8f;
		mLight.Ambient.r = AmbientValue;
		mLight.Ambient.g = AmbientValue;
		mLight.Ambient.b = AmbientValue;
		//光の方向.
		mLight.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

		//Direct3Dに関する生成.
		CreateDirectX9(apHwnd,aIsFullScreen);
	}
	void DirectGraphics::SetCamera(const CameraSystem& aCamera)
	{
		//ビルボードなどで使うためクラス内に保持
		mCamera = aCamera;

		//ビュー行列設定.
		mpD3DDevice->SetTransform(D3DTS_VIEW, &mCamera.GetViewMatrix());
		
		//射影行列設定
		D3DXMATRIX proj;
		auto ang = aCamera.GetViewingAngle();
		const float Aspect = CastFloat(WindowWidth) / CastFloat(WindowHeight);
		D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(ang), Aspect, 1.0f, 1000.0f);

		//行列設定
		Get3D().GetDevice()->SetTransform(D3DTS_PROJECTION, &proj);
	}

	void DirectGraphics::CreateDirectX9(HWND apHwnd, bool aIsFullScreen)
	{
		// Direct3D オブジェクトを作成
		mpD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (mpD3D == nullptr)
		{
			// オブジェクト作成失敗
			MessageBox(NULL, "Direct3D の作成に失敗しました。", "ERROR", MB_OK | MB_ICONSTOP);
		}
		int adapter;

		// 使用するアダプタ番号
		adapter = D3DADAPTER_DEFAULT;

		// ウインドウの作成が完了したので、Direct3D を初期化する
		ZeroMemory(&mD3Dpp, sizeof(D3DPRESENT_PARAMETERS));
		// Direct3D 初期化パラメータの設定
		if (aIsFullScreen) {
			mD3Dpp.Windowed = FALSE;
			mD3Dpp.BackBufferWidth = WindowWidth;
			mD3Dpp.BackBufferHeight = WindowHeight;
			mD3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
			mD3Dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		}
		// ウィンドウモード時
		else {
			mD3Dpp.Windowed = TRUE;
			mD3Dpp.BackBufferWidth = 0;
			mD3Dpp.BackBufferHeight = 0;
			// ウインドウ : 現在の画面モードを使用
			D3DDISPLAYMODE disp;
			// 現在の画面モードを取得
			mpD3D->GetAdapterDisplayMode(adapter, &disp);
			mD3Dpp.BackBufferFormat = disp.Format;
		}

		mD3Dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

		// Z バッファの自動作成
		mD3Dpp.EnableAutoDepthStencil = 1;
		mD3Dpp.AutoDepthStencilFormat = D3DFMT_D16;

		//ﾊﾞｯｸﾊﾞｯﾌｧをﾛｯｸ可能にする(GetDCも可能になる)
		mD3Dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		// デバイスの作成 - T&L HAL
		if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, apHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
		{
			// 失敗したので HAL で試行
			if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, apHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
			{
				// 失敗したので REF で試行
				if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_REF, apHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
				{
					// 結局失敗した
					MessageBox(NULL, "DirectX9が初期化できません。\n未対応のパソコンと思われます。", "ERROR", MB_OK | MB_ICONSTOP);
					mpD3D->Release();
				}
			}
		}

		//スプライトの生成と初期化
		mpSprite = std::make_shared<Sprite>();
		mpSprite->Initialize();

		// レンダリング・ステートを設定
		// Z バッファ有効化->前後関係の計算を正確にしてくれる
		mpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		mpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		mpD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);


		// アルファブレンディング有効化
		mpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		// アルファブレンディング方法を設定
		mpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// レンダリング時のアルファ値の計算方法の設定
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		// テクスチャの色を使用
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		// 頂点の色を使用
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		// レンダリング時の色の計算方法の設定
		mpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		//裏面カリング
		mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// フィルタ設定
		mpD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		mpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		mpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);

		// ライト
		mpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		//ライトをセット.
		mpD3DDevice->SetLight(0, &mLight);

		//ライトを有効.
		mpD3DDevice->LightEnable(0, TRUE);

		mpD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		// 頂点フォーマットの設定
		mpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	}
}