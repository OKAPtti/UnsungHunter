/**
* @file ArsFireWall.h
* @brief ���̕ǂ𐶐�����N���X
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class Texture;
	class FireWall :public GameObject
	{
	public:
		FireWall(void);
		~FireWall(void)override;

		/**
		* @brief �����������܂�
		* @param �\������ʒu
		*/
		void Initialize(const D3DXVECTOR3& aPosition);
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime)override;

	private:
		WaitTimeManager mDelayTime;

		/**
		* @brief ���̕ǂ𐶐����܂�
		*/
		void CreateFragment(void);

		//���������������L�^.
		int mCreateCount;
		//��h�������ʒu.
		D3DXVECTOR3 mPosition;
		//�O���FireWall�̈ʒu.
		D3DXVECTOR3 mPreviousPosition;
	};

}