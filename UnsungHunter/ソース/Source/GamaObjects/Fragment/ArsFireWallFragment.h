/**
* @file ArsFireWallFragment.h
* @brief ���̕ǂ𐧌䂷��N���X
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class FireWallFragment :public GameObject
	{
	public:
		FireWallFragment(void);
		~FireWallFragment(void)override;

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
		/**
		* @brief 3D�`������܂�
		*/
		void Draw3D(void)override;

		/**
		* @brief �|���S�����C��������܂�
		* @param �����蔻��Ɏg�p������
		* @param �����蔻��̌��ʂ�Ԃ�
		*/
		bool HitCheckRayPoly(const InputRayPoly& in, OutputRayPoly& out)override;
	private:
		WaitTimeManager mWaitTimeManager;

		/**
		* @brief �A�j���[�V�������X�V���܂�
		*/
		void UpdateAnimation(void);

		/**
		* @brief ���̕ǂ��폜���܂�
		*/
		void Delete(void);

		Vertex mVertex[4];

		//�r���[�{�[�h�Ȃ̂œ����蔻�������Ƃ��ɌX�����Ȃ���.
		D3DXMATRIX mSubMatrix;

		//���ݎg���摜.
		int mTextureNumber;
	};

}