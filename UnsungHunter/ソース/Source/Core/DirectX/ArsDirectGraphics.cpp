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
		//���C�g�̐ݒ�.
		ZeroMemory(&mLight, sizeof(D3DLIGHT9));
		mLight.Type = D3DLIGHT_DIRECTIONAL;	
		//�F�̐ݒ�(0�`1).
		//�g�U��.
		mLight.Diffuse.r = 1.0f;
		mLight.Diffuse.g = 1.0f;
		mLight.Diffuse.b = 1.0f;
		//����.
		constexpr float AmbientValue = 0.8f;
		mLight.Ambient.r = AmbientValue;
		mLight.Ambient.g = AmbientValue;
		mLight.Ambient.b = AmbientValue;
		//���̕���.
		mLight.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);

		//Direct3D�Ɋւ��鐶��.
		CreateDirectX9(apHwnd,aIsFullScreen);
	}
	void DirectGraphics::SetCamera(const CameraSystem& aCamera)
	{
		//�r���{�[�h�ȂǂŎg�����߃N���X���ɕێ�
		mCamera = aCamera;

		//�r���[�s��ݒ�.
		mpD3DDevice->SetTransform(D3DTS_VIEW, &mCamera.GetViewMatrix());
		
		//�ˉe�s��ݒ�
		D3DXMATRIX proj;
		auto ang = aCamera.GetViewingAngle();
		const float Aspect = CastFloat(WindowWidth) / CastFloat(WindowHeight);
		D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(ang), Aspect, 1.0f, 1000.0f);

		//�s��ݒ�
		Get3D().GetDevice()->SetTransform(D3DTS_PROJECTION, &proj);
	}

	void DirectGraphics::CreateDirectX9(HWND apHwnd, bool aIsFullScreen)
	{
		// Direct3D �I�u�W�F�N�g���쐬
		mpD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (mpD3D == nullptr)
		{
			// �I�u�W�F�N�g�쐬���s
			MessageBox(NULL, "Direct3D �̍쐬�Ɏ��s���܂����B", "ERROR", MB_OK | MB_ICONSTOP);
		}
		int adapter;

		// �g�p����A�_�v�^�ԍ�
		adapter = D3DADAPTER_DEFAULT;

		// �E�C���h�E�̍쐬�����������̂ŁADirect3D ������������
		ZeroMemory(&mD3Dpp, sizeof(D3DPRESENT_PARAMETERS));
		// Direct3D �������p�����[�^�̐ݒ�
		if (aIsFullScreen) {
			mD3Dpp.Windowed = FALSE;
			mD3Dpp.BackBufferWidth = WindowWidth;
			mD3Dpp.BackBufferHeight = WindowHeight;
			mD3Dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
			mD3Dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		}
		// �E�B���h�E���[�h��
		else {
			mD3Dpp.Windowed = TRUE;
			mD3Dpp.BackBufferWidth = 0;
			mD3Dpp.BackBufferHeight = 0;
			// �E�C���h�E : ���݂̉�ʃ��[�h���g�p
			D3DDISPLAYMODE disp;
			// ���݂̉�ʃ��[�h���擾
			mpD3D->GetAdapterDisplayMode(adapter, &disp);
			mD3Dpp.BackBufferFormat = disp.Format;
		}

		mD3Dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

		// Z �o�b�t�@�̎����쐬
		mD3Dpp.EnableAutoDepthStencil = 1;
		mD3Dpp.AutoDepthStencilFormat = D3DFMT_D16;

		//�ޯ��ޯ̧��ۯ��\�ɂ���(GetDC���\�ɂȂ�)
		mD3Dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

		// �f�o�C�X�̍쐬 - T&L HAL
		if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, apHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
		{
			// ���s�����̂� HAL �Ŏ��s
			if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, apHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
			{
				// ���s�����̂� REF �Ŏ��s
				if (FAILED(mpD3D->CreateDevice(adapter, D3DDEVTYPE_REF, apHwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &mD3Dpp, &mpD3DDevice)))
				{
					// ���ǎ��s����
					MessageBox(NULL, "DirectX9���������ł��܂���B\n���Ή��̃p�\�R���Ǝv���܂��B", "ERROR", MB_OK | MB_ICONSTOP);
					mpD3D->Release();
				}
			}
		}

		//�X�v���C�g�̐����Ə�����
		mpSprite = std::make_shared<Sprite>();
		mpSprite->Initialize();

		// �����_�����O�E�X�e�[�g��ݒ�
		// Z �o�b�t�@�L����->�O��֌W�̌v�Z�𐳊m�ɂ��Ă����
		mpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		mpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		mpD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);


		// �A���t�@�u�����f�B���O�L����
		mpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		// �A���t�@�u�����f�B���O���@��ݒ�
		mpD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		mpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		mpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// �����_�����O���̃A���t�@�l�̌v�Z���@�̐ݒ�
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		// �e�N�X�`���̐F���g�p
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		// ���_�̐F���g�p
		mpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		// �����_�����O���̐F�̌v�Z���@�̐ݒ�
		mpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

		//���ʃJ�����O
		mpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// �t�B���^�ݒ�
		mpD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		mpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
		mpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);

		// ���C�g
		mpD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		//���C�g���Z�b�g.
		mpD3DDevice->SetLight(0, &mLight);

		//���C�g��L��.
		mpD3DDevice->LightEnable(0, TRUE);

		mpD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
		// ���_�t�H�[�}�b�g�̐ݒ�
		mpD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	}
}