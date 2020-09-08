/**
* @file ArsModel.h
* @brief���f���𐧌䂷��N���X
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
		* @brief ���f����`�悵�܂�
		*/
		void Draw(void) {
			//���b�V�����Ȃ��Ȃ�`�悵�Ȃ�.
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
		* @brief ���f�����w�肵���A���t�@�l�ŕ`�悵�܂�
		* @param �A���t�@�l(0.0f�`1.0f).
		*/
		void Draw(float aAlphaValue) {
			//���b�V�����Ȃ��Ȃ�`�悵�Ȃ�.
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

		//�}�e���A������ύX���Ă����C�g(��)�̊֌W�őS�̂��Ԃ��Ȃ�Ȃ�����
		//���f���ɐԐF�̃e�N�X�`����\���Ă���.
		/**
		* @brief ���f����ԐF�ŕ`�悵�܂�
		*/
		void DrawRed(void) {
			//���b�V�����Ȃ��Ȃ�`�悵�Ȃ�.
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
		* @brief ���b�V����Ԃ��܂�
		*/
		const LPD3DXMESH GetMesh(void)const { return mpMesh.Get(); }

		/**
		* @brief �}�e���A���̃T�C�Y��Ԃ��܂�
		*/
		const UINT GetMaterialListNum(void)const { return mMaterialList.size(); }

		/**
		* @brief ���݂̃}�e���A�����X�g��Ԃ��܂�
		*/
		std::vector<D3DMATERIAL9> GetMaterialList(void) { return mMaterialList[mCurrentMaterialListIndex]; }
		const std::vector<D3DMATERIAL9>& GetMaterialList(void)const { return mMaterialList[mCurrentMaterialListIndex]; }
		
		/**
		* @brief �e�N�X�`�����X�g��Ԃ��܂�
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
