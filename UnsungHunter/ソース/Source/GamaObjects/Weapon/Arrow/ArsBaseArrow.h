#pragma once
#include "../ArsWeapon.h"

namespace Alrescha {

	class BaseArrow :public Weapon
	{
	public:
		BaseArrow(int aType);
		virtual ~BaseArrow(void)override;

		/**
		* @brief �����������܂�
		* @param ��΂�����
		* @param ����
		* @param ���ˈʒu
		*/
		virtual void Initialize(const D3DXVECTOR3& aShootAngle, float aPower, const D3DXVECTOR3& aShotPosition);
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
		/**
		* @brief ���������Ώە��ɒǔ����鏈�������܂�
		*/
		virtual void TrackingTarget(void);

		/**
		* @brief �ړ����������܂�
		*/
		virtual void MoveAction(float aDeltaTime);

		//��H���烌�C���΂��ۂ̈ʒu.
		D3DXVECTOR3 RayPosition;

		//������������̏��.
		WPtr<GameObject> mpHitObject;
		D3DXMATRIX mLocalRotate;
		D3DXVECTOR3 mLocalPosition;
		//��̑���.
		float mShootPower;

		/**
		* @brief �Փ˔�������܂�
		*/
		virtual void OnCollision(void)override;

		WaitTimeManager mWaitTimeManager;

		UPtr<class Effect> mpEffect;
	};
	
}