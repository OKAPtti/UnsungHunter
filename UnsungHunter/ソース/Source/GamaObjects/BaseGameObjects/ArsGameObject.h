/**
* @file ArsGameObject.h
* @brief �Q�[���ɓo�ꂷ��I�u�W�F�N�g�ɕK�v�Ȃ��̂��܂Ƃ߂��N���X
*/

#pragma once

#include "../ArsGameObjectType.h"

namespace Alrescha {
	class Scene;
	class InputSphere;
	class OutputSphere;
	class InputCircle;
	class OutputCircle;
	class InputRayPoly;
	class OutputRayPoly;
	class InputRay;
	class OutputRay;
	class Texture;
	class Model;
	class GameObject :public std::enable_shared_from_this<GameObject>
	{
	public:
		GameObject(int aType);
		virtual ~GameObject(void) {}


		/**
		* @brief �f�V���A���C�Y
		* @param Json�I�u�W�F�N�g
		*/
		virtual void Deserialize(const json11::Json& aJsonObject);
		/**
		* @brief �����������܂�
		*/
		virtual void Initialize(void);
		/**
		* @brief �X�V�����܂�
		* @param �f���^�^�C��
		*/
		virtual void Update(float aDeltaTime) {}
		/**
		* @brief 2D�`������܂�
		*/
		virtual void Draw2D(void) {}
		/**
		* @brief 3D�`������܂�
		*/
		virtual void Draw3D(void);

		//�����蔻��.
		/**
		* @brief ����������܂�
		* @param �����蔻��Ɏg�p������
		* @param �����蔻��̌��ʂ�Ԃ�
		*/
		bool HitCheckSphere(const InputSphere& in, OutputSphere& out);
		/**
		* @brief �~��������܂�
		* @param �����蔻��Ɏg�p������
		* @param �����蔻��̌��ʂ�Ԃ�
		*/
		bool HitCheckCircle(const InputCircle& in, OutputCircle& out);
		/**
		* @brief ���C��������܂�
		* @param �����蔻��Ɏg�p������
		* @param �����蔻��̌��ʂ�Ԃ�
		*/
		virtual bool HitCheckRay(const InputRay& in, OutputRay& out);
		/**
		* @brief �|���S�����C��������܂�
		* @param �����蔻��Ɏg�p������
		* @param �����蔻��̌��ʂ�Ԃ�
		*/
		virtual bool HitCheckRayPoly(const InputRayPoly& in, OutputRayPoly& out) { return false; }

		//�s��擾.
		/**
		* @brief SRT���������ꂽ�s���Ԃ��܂�
		* @note ���Q�ƕԂ��Ȃ̂ŕs�K�v�ɌĂ΂Ȃ�����
		*/
		D3DXMATRIX& GetMatrix(void)						{ return mMatrix; }
		/**
		* @brief SRT���������ꂽ�s���Ԃ��܂�
		*/
		const D3DXMATRIX& GetMatrixConst(void)const		{ return mMatrix; }
		/**
		* @brief �ړ��s���Ԃ��܂�
		*/
		const D3DXMATRIX& GetTransMatrix(void)const		{ return mTransMatrix; }
		/**
		* @brief ��]�s���Ԃ��܂�
		*/
		const D3DXMATRIX& GetRotateMatrix(void)const	{ return mRotateMatrix; }
		
		
		/**
		* @brief �s���ݒ肵�܂�
		* @param �ݒ肷��s��
		*/
		void SetMatrix(const D3DXMATRIX& aMatrix)		{ mMatrix = aMatrix; }

		/**
		* @brief ���W(�ʒu)��Ԃ��܂�
		*/
		D3DXVECTOR3 GetPosition(void)const				{return D3DXVECTOR3{ mTransMatrix._41,mTransMatrix._42,mTransMatrix._43 };}

		/**
		* @brief ���W(�ʒu)��ݒ肵�܂�
		* @param �ʒu��񂪊i�[����Ă���3D�x�N�g��
		*/
		void SetPosition(const D3DXVECTOR3& aPosition);
		/**
		* @brief ��]��ݒ肵�܂�
		* @param ��]��񂪐ݒ肳��Ă���s��
		*/
		void SetRotation(const D3DXMATRIX& aRotationMatrix);
		/**
		* @brief ��]��ݒ肵�܂�
		* @param ��]��񂪊i�[����Ă���3D�x�N�g��
		*/
		void SetRotation(const D3DXVECTOR3& aRadian);
		/**
		* @brief �g�k��ݒ肵�܂�
		* @param �g�k��񂪐ݒ肳��Ă���3D�x�N�g��
		*/
		void SetScale(const D3DXVECTOR3& aScale);

		/**
		* @brief ���W(�ʒu)�����Z
		* @param ���Z����3D�x�N�g��
		* @param ���[�J�����W�ɉ��Z���邩�ǂ���
		* @note �ݒ肳��Ă��Ȃ��ꍇ�̓��[�J�����W 
		*/
		void AddPosition(const D3DXVECTOR3& aAddPosition, bool aIsLocal = true);
		/**
		* @brief ��]�����Z
		* @param ���Z����3D�x�N�g��
		* @param ���[�J���ɉ��Z���邩�ǂ���
		* @note �ݒ肳��Ă��Ȃ��ꍇ�̓��[�J��
		*/
		void AddRotation(const D3DXMATRIX& aAddRotation, bool aIsLocal = true);

		/**
		* @brief �`�悷�邩�ǂ���
		* @param �`�悷�邩�̃t���O
		* @note true:�`�悷�� false:�`�悵�Ȃ�
		*/
		void SetIsVisible(bool aIsVisible)	{ mIsVisible = aIsVisible; }
		
		/**
		* @brief �`�悷�邩�ǂ����̃t���O��Ԃ��܂�
		*/
		bool GetIsVisible(void)const		{ return mIsVisible; }

		/**
		* @brief �����t���O��false�ɂ��܂�
		* @note Kill����ƍX�V���`�������Ȃ�
		* @note ����GameObject���A�N������Q�Ƃ���Ȃ��Ȃ�����delete���s��
		*/
		void Kill(void)				{ mIsAlive = false; }
		/**
		* @brief �����t���O��Ԃ��܂�
		*/
		bool GetIsAlive(void)const	{ return mIsAlive; }

		/**
		* @brief ���g�̃^�C�v��Ԃ��܂�
		*/
		int GetType(void)const { return mType; }

		/**
		* @brief �e��ݒ肵�܂�
		* @param �e�ɂ������I�u�W�F�N�g
		*/
		void SetParent(const SPtr<GameObject>& apParent) { mpParent = apParent; }

		/**
		* @brief �V�[����ݒ肵�܂�
		* @param �ݒ肵�����V�[��
		*/
		void SetScene(Scene* apScene)	{ if (apScene) mpScene = apScene; }

		/**
		* @brief ���g��(static)�L���X�g���܂�
		* @param �^�C�v
		* @note ���̃L���X�g���g���Ƃ��͈��������܂��Ă��鎞����.
		*/
		template<class T>
		SPtr<T> Cast(int aType);

		/**
		* @brief ���f���N���X��Ԃ��܂�
		*/
		Model* GetModel(void)const { return mpModel; }
		/**
		* @brief �e�N�X�`���N���X��Ԃ��܂�
		*/
		Texture* GetTexture(void)const { return mpTexture; }

	private:
		//���̃N���X���𔻒f.
		const int mType;

	protected:
		Scene*				mpScene;

		bool				mIsAlive;
		//�`�悷�邩�ǂ���.
		bool				mIsVisible;

		Texture*			mpTexture;
		Model*				mpModel;

		//�e�q�֌W.
		WPtr<GameObject>	mpParent;

		//�s��.
		D3DXMATRIX			mMatrix;
		D3DXMATRIX			mTransMatrix, mRotateMatrix, mScaleMatrix;

		//�Փ˔���.
		virtual void OnCollision(void) {}
	};

	//�^�C�v����ӂɌ��܂��Ă���ꍇ�̂ݎg�p��.
	template<class T>
	inline SPtr<T> GameObject::Cast(int aType)
	{
		//�����œn���ꂽ�^�C�v�Ɠ����ł���΃L���X�g.
		if (GetType() == aType) {
			return std::static_pointer_cast<T>(shared_from_this());
		}

		//����Ă�����.
		return nullptr;
	}

	//�ȉ������蔻��p.
	class InputSphere {
	public:
		D3DXVECTOR3 mCenterPos = { 0.0f,0.0f,0.0f };//�����蔻��̒��S.
		float mRadius = 0.0f;						//�����蔻��̔��a.
	};
	class OutputSphere {
	public:
		bool mIsHit = false;						//�����������ǂ���.
		float mDistance = 0.0f;						//�Q�_�Ԃ̋���.
		D3DXVECTOR3 mHitPos = { 0.0f,0.0f,0.0f };	//�Q�_�Ԃ̒��_
	};
	class InputCircle {
	public:
		D3DXVECTOR3 mCenterPos = { 0.0f,0.0f,0.0f };//�����蔻��̒��S.
		float mCollisionLength = 0.0f;				//�����蔻��̔��a.
	};
	class OutputCircle {
	public:
		bool mIsHit = false;						//�����������ǂ���.
		float mLength = 0.0f;						//�Q�_�Ԃ̋���.
		D3DXVECTOR3 mHitPos = { 0.0f,0.0f,0.0f };	//�Q�_�Ԃ̒��_
		D3DXVECTOR3 mForceOut = { 0.0f,0.0f,0.0f };	//�����o������.
	};
	class InputRayPoly {
	public:
		D3DXVECTOR3 mRayPos = { 0.0f,0.0f,0.0f };	//���C�̔��ˈʒu.
		D3DXVECTOR3 mRayDir = { 0.0f,0.0f,1.0f };	//���C�̔��˕���.
	};
	class OutputRayPoly {
	public:
		bool mIsHit = false;
		float mDistance = 0.0f;
	};
	class InputRay {
	public:
		D3DXVECTOR3 mRayPos = { 0.0f,0.0f,0.0f };	//���C�̔��ˈʒu.
		D3DXVECTOR3 mRayDir = { 0.0f,0.0f,1.0f };	//���C�̔��˕���.
	};
	class OutputRay {
	public:
		bool mIsHit = false;						//�����������ǂ���.
		float mDistance = 0.0f;						//�Q�_�Ԃ̋���.
		D3DXVECTOR3 mNormal = { 0.0f,0.0f,0.0f };	//���������|���S���̖@��.
		D3DXVECTOR3 mForceOut = { 0.0f,0.0f,0.0f };	//�����o������.
	};
}
