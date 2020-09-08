/**
* @file ArsQuiver.h
* @brief ��̓��ꕨ�N���X
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Quiver :public GameObject
	{
	public:
		Quiver(void);
		~Quiver(void)override;
		/**
		* @brief �����������܂�
		*/
		void Initialize(void)override final;
		/**
		* @brief �X�V���������܂�
		* @param �e�̍s��
		* @param �f���^�^�C��
		*/
		void Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime);
		/**
		* @brief 3D�`������܂�
		*/
		void Draw3D(void)override final;

	private:
	};

}