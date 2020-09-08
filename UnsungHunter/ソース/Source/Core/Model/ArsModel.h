/**
* @file ArsModel.h
* @briefモデルを制御するクラス
*/

#pragma once

#include "../DirectX/ArsDirectGraphics.h"
#include "../Texture/ArsTextureManager.h"

namespace Alrescha {

	class Model
	{
	public:
		Model(ComPtr<ID3DXMesh> apMesh,
			const std::vector<D3DMATERIAL9>& aMaterialList,
			const std::vector<LPDIRECT3DTEXTURE9>& aTextureList)
			: mpMesh(apMesh)
			, mCurrentMaterialListIndex(0)
			, mMaterialList({ {aMaterialList} })
			, mTextureList(aTextureList)
		{}
		~Model(void)
		{}

		/**
		* @brief モデルを描画します
		*/
		void Draw(void) {
			//メッシュがないなら描画しない.
			if (mpMesh == nullptr) { return; }


			LPD3DXMESH pMesh = GetMesh();
			auto materialList = GetMaterialList();
			auto textureList = GetTextureList();

			for (size_t i = 0; i < materialList.size(); i++) {
				Get3D().GetDevice()->SetMaterial(&materialList[i]);
				Get3D().GetDevice()->SetTexture(0, textureList[i]);
				pMesh->DrawSubset(i);
			}
		}
		
		/**
		* @brief モデルを指定したアルファ値で描画します
		* @param アルファ値(0.0f～1.0f).
		*/
		void Draw(float aAlphaValue) {
			//メッシュがないなら描画しない.
			if (mpMesh == nullptr) { return; }


			LPD3DXMESH pMesh = GetMesh();
			auto materialList = GetMaterialList();
			auto textureList = GetTextureList();

			D3DMATERIAL9 material;
			for (size_t i = 0; i < materialList.size(); i++) {
				material = materialList[i];
				material.Diffuse.a = aAlphaValue;
				Get3D().GetDevice()->SetMaterial(&material);
				Get3D().GetDevice()->SetTexture(0, textureList[i]);
				pMesh->DrawSubset(i);
			}
		}

		//マテリアル情報を変更してもライト(光)の関係で全体が赤くならないため
		//モデルに赤色のテクスチャを貼っている.
		/**
		* @brief モデルを赤色で描画します
		*/
		void DrawRed(void) {
			//メッシュがないなら描画しない.
			if (mpMesh == nullptr) { return; }


			LPD3DXMESH pMesh = GetMesh();
			auto materialList = GetMaterialList();
			auto textureList = GetTextureList();

			mpTexture = GetTexMgr().GetTexture("RedTexture.png");

			D3DMATERIAL9 material;
			for (size_t i = 0; i < materialList.size(); i++) {
				material = materialList[i];
				material.Emissive.a = 1.0f;
				material.Emissive.r = 0.6f;
				material.Emissive.g = 0.6f;
				material.Emissive.b = 0.6f;
				Get3D().GetDevice()->SetMaterial(&material);
				Get3D().GetDevice()->SetTexture(0, mpTexture->GetDirectTexture());
				pMesh->DrawSubset(i);
			}
		}

		/**
		* @brief メッシュを返します
		*/
		const LPD3DXMESH GetMesh(void)const { return mpMesh.Get(); }

		/**
		* @brief マテリアルのサイズを返します
		*/
		const UINT GetMaterialListNum(void)const { return mMaterialList.size(); }

		/**
		* @brief 現在のマテリアルリストを返します
		*/
		std::vector<D3DMATERIAL9> GetMaterialList(void) { return mMaterialList[mCurrentMaterialListIndex]; }
		const std::vector<D3DMATERIAL9>& GetMaterialList(void)const { return mMaterialList[mCurrentMaterialListIndex]; }
		
		/**
		* @brief テクスチャリストを返します
		*/
		const std::vector<LPDIRECT3DTEXTURE9>& GetTextureList(void)const { return mTextureList; }
	private:
		ComPtr<ID3DXMesh> mpMesh;
		int mCurrentMaterialListIndex;
		std::vector<std::vector<D3DMATERIAL9>> mMaterialList;
		std::vector<LPDIRECT3DTEXTURE9> mTextureList;

		Texture* mpTexture = nullptr;
	};
}
