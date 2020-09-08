/**
* @file ArsModelManager.h
* @brief���f���̓ǂݍ��݁A�Ǘ��N���X
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
		* @brief �����������܂�
		* @param �f�o�C�X
		*/
		void Initialize(LPDIRECT3DDEVICE9 apD3DDevice) { mpD3DDevice = apD3DDevice; }

		/**
		* @brief �I�����������܂�
		*/
		void Finalize(void);

		/**
		* @brief ���f����Ԃ��܂�
		* @param ���f����
		*/
		Model* GetModel(const std::string& aModelName);

	private:
		/**
		* @brief ���f����ǂݍ��݂܂�
		* @param ���f����
		*/
		UPtr<Model> LoadModel(const std::string& aModelName);

		/**
		* @brief �t�@�C�������݂��邩���ׂ܂�
		* @param �t�@�C���p�X
		*/
		bool ExistenceFile(const std::string& aPath)const;

		/**
		* @brief x�t�@�C���ǂݍ��݂܂�
		* @param ���f���f�[�^���i�[����Model�N���X
		* @param ���f���̃t�@�C���p�X
		* @param ���f����
		*/
		void LoadXFile(UPtr<Model>& aModel , const std::string aModelFilePath, const std::string& aName)const;

		//�f�o�C�X.
		LPDIRECT3DDEVICE9 mpD3DDevice;

		//Model�Ǘ��}�b�v.
		std::unordered_map <std::string, UPtr<Model>> mModelMap;

	protected:
		ModelManager(void);
		virtual ~ModelManager(void)override final;
	};
	inline ModelManager& GetModelMgr(void) { return ModelManager::GetInstance(); }
	#define ModelMgr ModelManager::GetInstance()
}