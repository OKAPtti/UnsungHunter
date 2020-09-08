/**
* @file ArsWall.h
* @brief �����Ȃ��ǃN���X
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Wall :public GameObject
	{
	public:
		Wall(void);
		~Wall(void)override final;
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
		* @brief ���C��������܂�
		* @param �����蔻��Ɏg�p������
		* @param �����蔻��̌��ʂ�Ԃ�
		*/
		bool HitCheckRay(const InputRay& in, OutputRay& out)override;

	};

}