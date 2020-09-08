/**
* @file ArsBoss.h
* @brief �{�X�N���X
*/


#pragma once
#include "ArsEnemy.h"

namespace Alrescha {

	class Boss :public Enemy
	{
	public:
		Boss(void);
		~Boss(void)override final;

		/**
		* @brief �����������܂�
		*/
		void Initialize(void)override final;

		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime)override final;
		/**
		* @brief 3D�`������܂�
		*/
		void Draw3D(void)override final;

	private:
		/**
		* @brief �Փ˔�������܂�
		*/
		void OnCollision(void);
		/**
		* @brief �I�I�J�~���X�|�[�����܂�
		*/
		void SpawnWolf(void);
		//�I�I�J�~������������.
		bool mIsSpawned;
	};

}