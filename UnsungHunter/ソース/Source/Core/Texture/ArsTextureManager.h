/**
* @file ArsTextureManager.h
* @brief �e�N�X�`���̓ǂݍ��݁A�Ǘ��N���X
*/

#pragma once

#include "ArsTexture.h"
#include "../ArsSingleton.h"

namespace Alrescha {
	class TextureManager:public Singleton<TextureManager> 
	{
	public:
		friend class Singleton<TextureManager>;

		/**
		* @brief �����������܂�
		* @param �f�o�C�X
		*/
		void Initialize(LPDIRECT3DDEVICE9 apD3DDevice) { mpD3DDevice = apD3DDevice; }
		
		/**
		* @brief �I�����������܂�
		*/
		void Finalize(void) { mTextureMap.clear(); }

		/**
		* @brief �e�N�X�`����Ԃ��܂�
		* @param �e�N�X�`���̖��O
		* @param �P�Ԗڂ̈������t���p�X���ǂ���(�w�肪�Ȃ��ꍇ��false)
		* @note aIsFullPath��true�Ȃ�e�N�X�`���t�H���_�܂ł̕␳�����Ȃ�
		*/
		Texture* GetTexture(const std::string& aName, bool aIsFullPath = false);

	private:
		//
		/**
		* @brief �摜��ǂݍ��݂܂�
		* @param �e�N�X�`�����i�[����Texture�N���X
		* @param �e�N�X�`���̖��O
		* @param �e�N�X�`���̖��O���t���p�X���ǂ���(�w�肪�Ȃ��ꍇ��false)
		*/
		void LoadTexture(UPtr<Texture>& aTexture, const std::string& aName, bool aIsFullPath = false);

		//�f�o�C�X.
		LPDIRECT3DDEVICE9 mpD3DDevice;
		//Texture�Ǘ��}�b�v.
		std::unordered_map<std::string, UPtr<Texture>> mTextureMap;
	
	protected:
		TextureManager(void);
		virtual ~TextureManager(void)override final;
	};
	inline TextureManager& GetTexMgr(void) { return TextureManager::GetInstance(); }

}