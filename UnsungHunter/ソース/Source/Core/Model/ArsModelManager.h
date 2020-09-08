/**
* @file ArsModelManager.h
* @briefモデルの読み込み、管理クラス
*/

#pragma once

#include "ArsModel.h"
#include "../ArsSingleton.h"

namespace Alrescha {

	class ModelManager:public Singleton<ModelManager>
	{
	public:
		friend class Singleton<ModelManager>;

		/**
		* @brief 初期化をします
		* @param デバイス
		*/
		void Initialize(LPDIRECT3DDEVICE9 apD3DDevice) { mpD3DDevice = apD3DDevice; }

		/**
		* @brief 終了処理をします
		*/
		void Finalize(void);

		/**
		* @brief モデルを返します
		* @param モデル名
		*/
		Model* GetModel(const std::string& aModelName);

	private:
		/**
		* @brief モデルを読み込みます
		* @param モデル名
		*/
		UPtr<Model> LoadModel(const std::string& aModelName);

		/**
		* @brief ファイルが存在するか調べます
		* @param ファイルパス
		*/
		bool ExistenceFile(const std::string& aPath)const;

		/**
		* @brief xファイル読み込みます
		* @param モデルデータを格納するModelクラス
		* @param モデルのファイルパス
		* @param モデル名
		*/
		void LoadXFile(UPtr<Model>& aModel , const std::string aModelFilePath, const std::string& aName)const;

		//デバイス.
		LPDIRECT3DDEVICE9 mpD3DDevice;

		//Model管理マップ.
		std::unordered_map <std::string, UPtr<Model>> mModelMap;

	protected:
		ModelManager(void);
		virtual ~ModelManager(void)override final;
	};
	inline ModelManager& GetModelMgr(void) { return ModelManager::GetInstance(); }
	#define ModelMgr ModelManager::GetInstance()
}