/**
* @file ArsEnemy.h
* @brief �G�ɕK�v�Ȃ��̂��W�߂��N���X
*/

#pragma once
#include "../ArsCharacter.h"

namespace Alrescha {

	class Enemy :public Character
	{
	public:
		Enemy(int aType);
		virtual ~Enemy(void);

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
		* @brief ������ݒ肵�܂�
		* @param �ݒ肵��������
		*/
		void SetHeight(float aHeight) { mHeight = aHeight; }
		/**
		* @brief ������Ԃ��܂�
		*/
		float GetHeight(void)const { return mHeight; }

		/**
		* @brief �m�b�N�o�b�N����͂�ݒ�
		* @param �m�b�N�o�b�N����������i�[���ꂽ3D�x�N�g��
		* @note KnockBackState�Ŏg�p���܂�
		*/
		void SetImpactDirection(const D3DXVECTOR3& aDirection) { mReceivedImpactDirection = aDirection; }
		/**
		* @brief �m�b�N�o�b�N����͂�Ԃ��܂�
		*/
		const D3DXVECTOR3& GetImpactDirection(void)const { return mReceivedImpactDirection; }
	protected:
		UPtr<class StateMachine> mpStateMachine;
		//���S����n�ʂ܂ł̋���.
		float mHeight;

		//���񂾎��ɁA���X�ɏ����Ă���(0.0f�`1.0f).
		float mAlphaValue;
	private:
		//�U�������������ꏊ.
		D3DXVECTOR3 mHitPosition;

		//�󂯂��Ռ�(�U��)�̕���.
		D3DXVECTOR3 mReceivedImpactDirection;
	};

}