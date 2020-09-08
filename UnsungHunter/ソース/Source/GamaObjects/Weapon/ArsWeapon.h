/**
* @file ArsWeapon.h
* @brief ����ɕK�v�Ȃ��̂��W�߂��N���X
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Weapon :public GameObject
	{
	public:
		Weapon(int aType);
		virtual ~Weapon(void);
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

	private:
	protected:
		//�^����_���[�W��.
		int mDamage;

		//��������.
		bool mIsHit;
	};
}