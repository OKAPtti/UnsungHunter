#include "ArsTextureManager.h"
#include "../ArsConfiguration.h"

namespace Alrescha {

	TextureManager::TextureManager(void)
		: mpD3DDevice(nullptr)
		, mTextureMap()
	{}

	TextureManager::~TextureManager(void)
	{}

	Texture* TextureManager::GetTexture(const std::string& aName, bool aIsFullPath)
	{
		auto it = mTextureMap.find(aName);
		if (it  == mTextureMap.end()){
			UPtr<Texture> texture;
			LoadTexture(texture, aName,aIsFullPath);
			mTextureMap[aName] = std::move(texture);

			return mTextureMap[aName].get();
		}
		return (*it).second.get();
	}

	void TextureManager::LoadTexture(UPtr<Texture>& aTexture, const std::string & aName, bool aIsFullPath)
	{
		if (!mpD3DDevice) {
			_ASSERT_EXPR(false, L"Initialize���Ă�ŉ�����");
		}

		LPDIRECT3DTEXTURE9 pTex = nullptr;
		std::string texturePath = aName;
		if (!aIsFullPath) {
			texturePath = FixedTexturePath + aName;
		}

		//�摜�̑傫���𒲂ׂ�
		D3DXIMAGE_INFO info;
		if (FAILED(D3DXGetImageInfoFromFile(texturePath.c_str(), &info))) {
			_ASSERT_EXPR(false, L"�e�N�X�`���̖��O���Ⴄ���A�摜�t�@�C�������Ă��܂�");
		}

		D3DXCreateTextureFromFileEx(mpD3DDevice, texturePath.c_str(),
			info.Width, info.Height,
			1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
			D3DX_FILTER_NONE, D3DX_DEFAULT,
			NULL, &info, NULL, &pTex);

		aTexture = std::make_unique<Texture>(pTex, info.Width, info.Height);
	}
}