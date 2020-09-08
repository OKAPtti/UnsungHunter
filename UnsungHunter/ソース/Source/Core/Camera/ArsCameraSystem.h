/**
* @file ArsCameraSystem.h
* @brief �J�����𐧌䂷��N���X
*/

#pragma once

namespace Alrescha {
	class GameObject;
	class CameraSystem
	{
	public:
		CameraSystem(void);
	   ~CameraSystem(void);

		
		/**
		* @brief ��ʉ�]�̍X�V
		* @param �}�E�X�̈ړ���
		*/
		void UpdateRotation(const D3DXVECTOR2& aAddRadian);

		
		/**
		* @brief �r���[�s��̍X�V
		*/
		void UpdateViewMatrix(void);

		/**
		* @brief �����_�̎w��
		* @param �������������I�u�W�F�N�g
		*/
		void SetTargetViewpoint(const WPtr<const GameObject>& apTargetViewpoint) { mpTargetViewpoint = apTargetViewpoint; }

		/**
		* @brief �r���[�s���Ԃ��܂�
		*/
		const D3DXMATRIX& GetViewMatrix(void)const;
		
		/**
		* @brief �����_�̍��W��Ԃ��܂�
		*/
		const D3DXVECTOR3& GetViewpoint(void);

		/**
		* @brief �J�����̉�]�s���Ԃ��܂�
		*/
		const D3DXMATRIX& GetCamRot(void);

		/**
		* @brief �J�������猩���x�N�g����Ԃ��܂�
		*/
		const D3DXVECTOR3& GetCenterVector(void)const;

		/**
		* @brief ����p��Ԃ��܂�
		*/
		float GetViewingAngle(void)const;

		/**
		* @brief ����p��Ԃ��܂�
		* @param �ړ�������x�N�g��(�ꏊ)
		* @param �I������
		*/
		void SetTransPosition(const D3DXVECTOR3& aTransPosition, int aEndCount);

		/**
		* @brief ��ʒ������g�債�܂�
		* @note ����p��ύX���A�g�傳���Ă���
		*/
		void ZoomIn(void);
		
		/**
		* @brief ��ʒ������k�����܂�
		* @note ����p��ύX���A�k�������Ă���
		*/
		void ZoomOut(void);

		/**
		* @brief �ʏ�̏�Ԃł̃}�E�X���x
		*/
		void NormalSensitivity(void)	{ mAdjustmentValue = 0.001f; }

		/**
		* @brief �|���\���Ă��鎞�̃}�E�X���x
		*/
		void BendSensitivity(void)		{ mAdjustmentValue = 0.0005f; }

	private:
		//�����_�̑Ώ�.
		WPtr<const GameObject>	mpTargetViewpoint;
		//�����_�̍��W.
		D3DXVECTOR3				mViewpoint;

		//�r���[�s��.
		D3DXMATRIX				mViewMatrix;
		//�J�����̉�]�s��.
		D3DXMATRIX				mCamRot;

		//�J�����̊p�x.
		D3DXVECTOR2				mCamAng;

		//�J�����̈ʒu���ꎞ�I�Ɋ��炩�ɕύX.
		D3DXVECTOR3				mTransPosition;
		Lerper<D3DXVECTOR3>		mTransPositionLerper;

		//�}�E�X���x.
		float					mSensitivity;
		//�}�E�X���x�̒����l.
		float					mAdjustmentValue;

		//����p.
		float					mViewingAngle;
		Lerper<float>			mAngleLerper;
	};

}