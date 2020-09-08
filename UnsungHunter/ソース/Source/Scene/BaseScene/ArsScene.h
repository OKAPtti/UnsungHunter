/**
* @file ArsScene.h
* @brief �V�[���̊��N���X
*/

#pragma once

#include "../../Core/ArsSingleton.h"
#include "ArsRayInfo.h"
#include "../ArsSceneType.h"

namespace Alrescha {
	class GameFrame;
	class GameObject;
	class CameraSystem;
	class Scene
	{
	public:
		Scene(GameFrame* apGameFrame);
		virtual ~Scene(void);
		
		/**
		* @brief �V�[���̏����������܂�
		* @param Json�t�@�C����
		*/
		virtual void InitializeScene(const std::string& aJsonFileName);

		/**
		* @brief �X�V�����܂�
		*/
		virtual void Update(void);
		/**
		* @brief �`������܂�
		*/
		virtual void Draw(void);

		/**
		* @brief �V�[���^�C�v��Ԃ��܂�
		*/
		const SceneType& GetType(void)const { return mType; }

		/**
		* @brief �V�[����GameObject��ǉ����܂�
		* @param �ǉ�������GameObject
		* @param �`�揇
		* @param aDrawOrder��0�`9�̊ԂŁA�������傫������ɕ`�悳���
		*/
		void AddGameObject(SPtr<GameObject> apGameObject, UINT aDrawOrder = 4U);

		/**
		* @brief �w�肵�������^�C�v��GameObject���X�g��Ԃ��܂�
		* @param �擾�������^�C�v
		* @note �s���|�C���g�Ŏ擾�������Ƃ�
		*/
		std::list<SPtr<GameObject>> FindGameObjectList(int aType);
		/**
		* @brief GameObject��Ԃ��܂�
		* @param �擾�������^�C�v
		* @note �s���|�C���g�Ŏ擾�������Ƃ�
		*/
		SPtr<GameObject> FindGameObject(int aType);
		/**
		* @brief �w�肵���͈͂�GameObject���X�g��Ԃ��܂�
		* @param �擾���������߂̃^�C�v
		* @param �擾�������I���̃^�C�v
		* @note ����͈͓��Ŏ擾�������Ƃ�.
		*/
		std::list<SPtr<GameObject>> FindGameObjectList(int aBeginType, int aEndType);

		/**
		* @brief ���b�V���Ƃ̓����蔻���Ԃ��܂�
		* @param ���b�V�����
		* @param �s��
		* @param ���C�̔��ˈʒu
		* @param ���C�̔��˕���
		* @param �����������C�̋���
		* @note �����Ă���Ȃ�true��Ԃ�
		*/
		BOOL Intersect(LPD3DXMESH& aMesh, const D3DXMATRIX& aAnotherMatrix
			, const D3DXVECTOR3& aRayShotPos, const D3DXVECTOR3& aRayDirection,
			float* aDistance);

		/**
		* @brief �����������X�g��Ԃ��܂�
		* @note �������߂����Ƀ\�[�g����܂�
		*/
		std::list<RayInfo> CameraRayTestList(void);

		/**
		* @brief RayInfo��Ԃ��܂�
		* @param ���C�̔��ˈʒu
		* @param ���C�̔��˕���
		* @param ���肵�����^�C�v
		*/
		RayInfo RayTest(const D3DXVECTOR3& aPosition,
			const D3DXVECTOR3& aRayDirection, int aJudgeType);

		/**
		* @brief �f���^�^�C����Ԃ��܂�
		*/
		float GetDeltaTime(void)const;

		/**
		* @brief �J������ݒ肵�܂�
		*/
		void SetTargetCamera(const SPtr<GameObject>& aTargetObject) { mTargetCamera = aTargetObject; }

		/**
		* @brief �J������Ԃ��܂�
		*/
		CameraSystem* GetCamera(void) { return mpCamera.get(); }

	private:
		GameFrame* mpGameFrame;
		WPtr<GameObject> mTargetCamera;
		UPtr<CameraSystem> mpCamera;

	protected:
		std::vector<std::list<SPtr<GameObject>>> mGameObjectList;

		//���ǂ̃V�[�����𔻒f����.
		SceneType mType;

		/**
		* @brief �V�[���^�C�v��ݒ肵�܂�
		*/
		void SetType(const SceneType& aType) { mType = aType; }
	};
}