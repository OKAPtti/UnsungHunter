/**
* @file ArsTextureManager.h
* @brief テクスチャの読み込み、管理クラス
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
		* @brief 初期化をします
		* @param デバイス
		*/
		void Initialize(LPDIRECT3DDEVICE9 apD3DDevice) { mpD3DDevice = apD3DDevice; }
		
		/**
		* @brief 終了処理をします
		*/
		void Finalize(void) { mTextureMap.clear(); }

		/**
		* @brief テクスチャを返します
		* @param テクスチャの名前
		* @param １番目の引数がフルパスかどうか(指定がない場合はfalse)
		* @note aIsFullPathがtrueならテクスチャフォルダまでの補正をしない
		*/
		Texture* GetTexture(const std::string& aName, bool aIsFullPath = false);

	private:
		//
		/**
		* @brief 画像を読み込みます
		* @param テクスチャを格納するTextureクラス
		* @param テクスチャの名前
		* @param テクスチャの名前がフルパスかどうか(指定がない場合はfalse)
		*/
		void LoadTexture(UPtr<Texture>& aTexture, const std::string& aName, bool aIsFullPath = false);

		//デバイス.
		LPDIRECT3DDEVICE9 mpD3DDevice;
		//Texture管理マップ.
		std::unordered_map<std::string, UPtr<Texture>> mTextureMap;
	
	protected:
		TextureManager(void);
		virtual ~TextureManager(void)override final;
	};
	inline TextureManager& GetTexMgr(void) { return TextureManager::GetInstance(); }

}