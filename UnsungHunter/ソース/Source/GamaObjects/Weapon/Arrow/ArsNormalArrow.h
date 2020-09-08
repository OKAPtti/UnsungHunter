/**
* @file ArsNormalArrow.h
* @brief �ʏ��N���X
*/

#pragma once
#include "ArsBaseArrow.h"

namespace Alrescha {

	class NormalArrow :public BaseArrow
	{
	public:
		NormalArrow(void);
		~NormalArrow(void)override;

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
		/**
		* @brief �Փ˔�������܂�
		*/
		void OnCollision(void)override;
	};
}