/**
* @file ArsBow.h
* @brief �|�N���X
*/

#pragma once
#include "ArsWeapon.h"
#include "../Character/ArsArrowType.h"

namespace Alrescha {

	class Bow :public Weapon
	{
	public:
		Bow(void);
		~Bow(void)override;
		/**
		* @brief �����������܂�
		*/
		void Initialize(void)override;
		/**
		* @brief �X�V���������܂�
		* @param �e�̍s��
		* @param �f���^�^�C��
		*/
		void Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime);
		/**
		* @brief 3D�`������܂�
		*/
		void Draw3D(void)override;
		/**
		* @brief ��𐶐����܂�
		*/
		void Shot(const ArrowType& aArrowType);

	private:
		D3DXMATRIX mShiftMatrix;
	};

}