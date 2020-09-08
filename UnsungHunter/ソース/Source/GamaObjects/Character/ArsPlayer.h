/**
* @file ArsCharacter.h
* @brief �v���C���[�N���X
*/

#pragma once
#include "ArsCharacter.h"
#include "ArsArrowType.h"

namespace Alrescha {
	class ChildGameObject;
	class Bow;
	class Quiver;
	class Player :public Character
	{
	public:
		Player(void);
		~Player(void)override;

		/**
		* @brief �f�V���A���C�Y
		* @param Json�I�u�W�F�N�g
		*/
		void Deserialize(const json11::Json& aJsonObject)override;
		/**
		* @brief �����������܂�
		*/
		void Initialize(void)override;
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		void Update(float aDeltaTime);
		/**
		* @brief 3D�`������܂�
		*/
		void Draw3D(void);

		//���擾.
		/**
		* @brief �ړ����Ă��邩��Ԃ��܂�
		*/
		bool GetIsMove(void)const		{ return mIsMove; }
		/**
		* @brief �|���\���Ă��邩��Ԃ��܂�
		*/
		bool GetIsBendBow(void)const	{ return mIsBendBow; }
		/**
		* @brief �������Ă��邩��Ԃ��܂�
		*/
		bool GetIsShootArrow(void)const { return mIsShootArrow; }
		/**
		* @brief ��̃^�C�v��Ԃ��܂�
		*/
		const ArrowType& GetArrowType(void)const		{ return mSelectedArrowType; }
		/**
		* @brief ���̖�̐���Ԃ��܂�
		*/
		const UPtr<Clamp<int>>& GetNumberOfArrows(void) { return mpNumOfArrowsForFire; }
	private:
		/**
		* @brief �ړ�(�֘A)�������܂�
		*/
		void MoveAction(float aDeltaTime);

		/**
		* @brief ��̐؂�ւ�����
		*/
		void SelectArrowAction(void);
		
		/**
		* @brief �I�����Ă����̎c����1�ȏォ�ǂ���
		*/
		bool NumberOfSelectArrows(void);

		/**
		* @brief �U������.
		*/
		void AttackAction(void);

		/**
		* @brief �X�^�~�i���񕜂����܂�
		*/
		void StaminaPlus(void);
		/**
		* @brief �X�^�~�i�����������܂�
		*/
		void StaminaMinus(void);
		/**
		* @brief �X�^�~�i�Ɋւ��鏈�������܂�
		*/
		void StaminaManagement(void);

		//�q.
		SPtr<ChildGameObject>	mpChild;
		SPtr<Bow>				mpBow;
		SPtr<Quiver>			mpQuiver;

		WaitTimeManager			mWaitTimeManager;

		bool					mIsMove;
		//�|���\���Ă��邩.
		bool					mIsBendBow;
		//�����Ƃ��Ƃ��Ă��邩.
		bool					mIsShootArrow;

		//���I�����Ă����̎��.
		ArrowType				mSelectedArrowType;

		//���̖�̏�����.
		UPtr<Clamp<int>>		mpNumOfArrowsForFire;

		/**
		* @brief �Փ˔�������܂�
		*/
		void OnCollision(void)override;
	};

}