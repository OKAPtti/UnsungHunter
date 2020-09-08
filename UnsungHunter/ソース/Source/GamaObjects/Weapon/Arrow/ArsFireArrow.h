/**
* @file ArsFireArrow.h
* @brief ���̖�N���X
*/

#pragma once
#include "ArsBaseArrow.h"

namespace Alrescha {

	class FireArrow :public BaseArrow
	{
	public:
		FireArrow(void);
		~FireArrow(void)override;

		/**
		* @brief �����������܂�
		* @param ��΂�����
		* @param ����
		* @param ���ˈʒu
		*/
		void Initialize(const D3DXVECTOR3& aShootAngle, float aPower, const D3DXVECTOR3& aShotPosition)override;
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime)override;
		/**
		* @brief 3D�`������܂�
		*/
		void Draw3D(void)override;
	private:
		//���𐶐�������.
		bool mIsCreateFireWall;

		/**
		* @brief �Փ˔�������܂�
		*/
		void OnCollision(void)override;

		//�p���_���[�W.
		WaitTimeManager mWaitTimeManager;
	};

}