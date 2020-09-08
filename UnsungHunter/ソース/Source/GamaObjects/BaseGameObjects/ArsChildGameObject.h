/**
* @file ArsChildGameObject.h
* @brief �q�N���X
*/

#pragma once
#include "ArsGameObject.h"

namespace Alrescha {

	class ChildGameObject :public GameObject
	{
	public:
		ChildGameObject(int aType);
		virtual ~ChildGameObject(void);

		/**
		* @brief �����������܂�
		* @param ���f����
		*/
		virtual void Initialize(const std::string& aModelName);
		/**
		* @brief �s����X�V
		* @param �e�̍s��
		* @param �f���^�^�C��
		*/
		virtual void UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime) {};
		/**
		* @brief �s����X�V
		* @param �e�̍s��
		* @param �f���^�^�C��
		*/
		void Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime);
		/**
		* @brief �����A�j���[�V���������܂�
		*/
		virtual void WalkAnimation(void);

		/**
		* @brief �`������܂�
		* @param �A���t�@�l
		* @note ���������̏ꍇ��1.0f
		*/
		virtual void Draw(float aAlphaValue = 1.0f);
		/**
		* @brief �ԐF�ɕ`������܂�
		*/
		virtual void DrawRed(void);

		/**
		* @brief �q��ǉ����܂�
		* @param �ǉ��������q���X�g
		*/
		void AddChild(SPtr<ChildGameObject> apAddChildList);
		/**
		* @brief �q��ǉ����܂�
		* @param �ǉ��������q���X�g
		*/
		void AddChild(std::list<SPtr<ChildGameObject>> apAddChildList);

		/**
		* @brief �S�Ă̎q�������̃��X�g�ɕԂ��܂�
		* @param �q���i�[���郊�X�g
		*/
		void GetChildList(std::list<SPtr<ChildGameObject>>& aAnotherList);
		
		/**
		* @brief �w�肵���q�������̃��X�g�ɕԂ��܂�
		* @param �q���i�[���郊�X�g
		* @param �擾�������^�C�v
		*/
		void GetChild(std::list<SPtr<ChildGameObject>>& aAnotherList, int aType);

		/**
		* @brief �S�Ă̎q�̃A�j���[�V�����t���[����0�ɂ��܂�
		*/
		void AllAnimeFrameReset(void);
	private:
		//�q.
		std::list<SPtr<ChildGameObject>> mChildList;
	protected:
		//�A�j���[�V��������Ƃ��ɂ̃t���[����.
		float							 mAnimeFrame;

	};
}