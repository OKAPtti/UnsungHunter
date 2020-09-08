/**
* @file ArsEffect.h
* @brief �G�t�F�N�g�N���X
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Effect :public GameObject
	{
	public:
		Effect(int aType);
		virtual ~Effect(void)override;

		/**
		* @brief �����������܂�
		*/
		virtual void Initialize(void);

		/**
		* @brief �s����X�V
		* @param �f���^�^�C��
		* @param ���O�擾����s��.
		*/
		virtual void UpdateTransform(float aDeltaTime, const D3DXMATRIX& aMatrix);
		/**
		* @brief 3D�`������܂�
		*/
		virtual void Draw3D(void)override;

		/**
		* @brief ���O���X�g��擪���珇�ɍ폜���܂�
		*/
		void RemoveLogList(void);

		/**
		* @brief ���_���W�̐ݒ肵�܂�
		* @param �ݒ肵�������_�̍��W
		* @note �w�肵�Ȃ��ꍇ��{ 0.1f,0.1f,0.0f }
		*/
		void SetVertexPos(const D3DXVECTOR3& aPosition = { 0.1f,0.1f,0.0f });
		
		/**
		* @brief �S���_�̐F����ݒ肵�܂�
		* @param �F
		*/
		void SetVertexColor(D3DCOLOR aColor);
		
		/**
		* @brief ���O���擾����T�C�Y��ݒ肵�܂�
		* @param �w�肵�����T�C�Y
		*/
		void SetLogSize(int aSize) { mMaxListSize = aSize; }
	private:
		//���O���i�[���郊�X�g.
		std::list<D3DXMATRIX> mLogList;
		//�|���S���̒��_�����i�[���郊�X�g.
		std::array<Vertex, 4> mVertexList;

		//�擾���郍�O�̍ő吔.
		int mMaxListSize;
	};

}