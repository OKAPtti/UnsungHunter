/**
* @file ArsCharacter.h
* @brief �L�����N�^�[�ɕK�v�Ȃ��̂��W�߂��N���X
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class Character :public GameObject
	{
	public:
		Character(int aType);
		~Character(void)override;
		/**
		* @brief �����������܂�
		*/
		virtual void Initialize(void);
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		virtual void Update(float aDeltaTime);
		/**
		* @brief 3D�`������܂�
		*/
		virtual void Draw3D(void);

		/**
		* @brief �_���[�W��^���܂�
		* @param �_���[�W��
		* @param �U���̖��O
		*/
		void Damage(UINT aDamageValue, const std::string& aDamageName);

		/**
		* @brief �񕜂����܂�
		* @param �񕜗�
		*/
		void Cure(UINT aCureValue);
		/**
		* @brief HP��Ԃ��܂�
		*/
		const SPtr<Clamp<int>>& GetHp(void)const { return mpHitPoint; }
		/**
		* @brief �X�^�~�i��Ԃ��܂�
		*/
		const SPtr<Clamp<float>>& GetStamina(void)const { return mpStamina; }
		
		/**
		* @brief Hit�t���O��true�ɂ��܂�
		*/
		void Hit(void) { mIsHit = true; }
		/**
		* @brief Hit�t���O��false�ɂ��܂�
		*/
		void NotIsHit(void) { mIsHit = false; }
		/**
		* @brief Hit�t���O��Ԃ��܂�
		*/
		bool GetIsHit(void)const { return mIsHit; }

	protected:
		SPtr<class ChildGameObject> mpChild;

		SPtr<Clamp<int>> mpHitPoint;
		SPtr<Clamp<float>> mpStamina;

		//���G����.
		WaitTimeManager mInvincibleTime;
		//���G��Ԃ��ǂ���.
		bool mIsInvincible;

		//�U����H�������.
		bool mIsHit;
	};

}