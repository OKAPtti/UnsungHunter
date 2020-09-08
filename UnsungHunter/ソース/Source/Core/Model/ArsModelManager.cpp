#include <fstream>

#include "ArsModelManager.h"
#include "../ArsConfiguration.h"
#include "../Texture/ArsTextureManager.h"

//メモリリーク検出用.
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace Alrescha {
	ModelManager::ModelManager(void)
		: mpD3DDevice(nullptr)
		, mModelMap()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}


	ModelManager::~ModelManager(void)
	{
	}

	void ModelManager::Finalize(void)
	{
		mModelMap.clear();
	}

	Model * ModelManager::GetModel(const std::string & aModelName)
	{
		auto it = mModelMap.find(aModelName);
		if (it == mModelMap.end()){
			mModelMap[aModelName] = LoadModel(aModelName);
			return mModelMap[aModelName].get();
		}
		return  (*it).second.get();
	}

	UPtr<Model> ModelManager::LoadModel(const std::string & aModelName)
	{
		//モデルまでのパスを合成.
		std::string ModelFilePath(FixedModelPath + aModelName + FixedModelFilePath
			+ aModelName);

		//指定されたファイルが存在するか.
		bool isXFile = ExistenceFile(ModelFilePath + ".x");

		UPtr<Model> pModel;

		if (isXFile) {
			LoadXFile(pModel, ModelFilePath + ".x", aModelName);
		}
		else {
			_ASSERT_EXPR(false, L"モデルの名前が違います");
		}

		return pModel;
	}

	bool ModelManager::ExistenceFile(const std::string & aPath)const
	{
		std::ifstream tmp(aPath);
		return tmp.is_open();
	}

	void ModelManager::LoadXFile(UPtr<Model>& aModel, const std::string aModelFilePath, const std::string & aName) const
	{
		if (!mpD3DDevice) {
			_ASSERT_EXPR(false, L"Initializeが呼ばれていません");
		}

		LPD3DXBUFFER pD3DXBuffer;
		DWORD materialNum;
		LPD3DXMESH pMesh;

		if (S_OK != D3DXLoadMeshFromX(aModelFilePath.c_str(), D3DXMESH_MANAGED,
			mpD3DDevice, NULL, &pD3DXBuffer, NULL, &materialNum, &pMesh)) {
			_ASSERT_EXPR(false, L"XFileの中身がおかしい可能性があります");
		}

		std::vector<D3DMATERIAL9> materialList(materialNum);
		std::vector<LPDIRECT3DTEXTURE9> textureList(materialNum);

		D3DXMATERIAL* D3DXMat = static_cast<D3DXMATERIAL*>(pD3DXBuffer->GetBufferPointer());


		if (materialNum > 0) {
			for (DWORD i = 0; i < materialNum; i++) {
				materialList[i] = D3DXMat[i].MatD3D;
				materialList[i].Ambient = materialList[i].Diffuse;

				if (D3DXMat[i].pTextureFilename) {

					const std::string ModelTexturePath(FixedModelPath + aName +
						FixedModelTexturePath + D3DXMat[i].pTextureFilename);
					textureList[i] = TextureManager::GetInstance().GetTexture(
						ModelTexturePath, true)->GetDirectTexture();
				}
				else
				{
					textureList[i] = nullptr;
				}
			}
		}
		else {
			//適当にマテリアルを入れておく
			D3DMATERIAL9 material;
			D3DCOLORVALUE color;
			color.a = color.b = color.g = color.r = 1.0f;
			material.Diffuse = color;
			materialList.push_back(material);

			textureList.push_back(LPDIRECT3DTEXTURE9(nullptr));
		}

		pD3DXBuffer->Release();

		aModel.reset(new Model(pMesh, materialList, textureList));

	}
}