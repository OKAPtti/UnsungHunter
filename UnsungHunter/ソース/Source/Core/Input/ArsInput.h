/**
* @file ArsInput.h
* @brief �}�E�X��L�[�{�[�h�𐧌䂷��N���X
*/

#pragma once

#include "../ArsSingleton.h"

namespace Alrescha {

	class Input :public Singleton<Input> 
	{
	public:
		friend class Singleton<Input>;

		/**
		* @brief ���������܂�
		* @param �E�B���h�E�n���h��
		*/
		void Initialize(HWND apHwnd);

		/**
		* @brief �X�V���܂�
		*/
		void Update(void);

		/**
		* @brief �E�B���h�E�Ƀ��b�Z�[�W�𑗂�܂�
		* @param MSG
		* @param WPARAM
		* @param LPARAM
		*/
		void SetWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		/**
		* @brief �L�[��������Ă�����true��Ԃ��܂�
		*/
		constexpr bool GetKey(char aKeyCode)const		{ return mDownKeyList[aKeyCode]; }
		/**
		* @brief �L�[�������ꂽ1�t���[������true��Ԃ��܂�

		*/
		constexpr bool GetKeyDown(char aKeyCode)const	{ return mDownKeyList[aKeyCode] && !mPreviousKeyList[aKeyCode]; }
		
		/**
		* @brief �L�[�������ꂽ1�t���[������true��Ԃ��܂�
		*/
		constexpr bool GetKeyUp(char aKeyCode)const		{ return !mDownKeyList[aKeyCode] && mPreviousKeyList[aKeyCode]; }
		/**
		* @brief �}�E�X�̍��W��Ԃ��܂�
		*/
		const D3DXVECTOR2 GetMousePosition(void)const	{ return mMousePosition; }
		
		/**
		* @brief �}�E�X�̑O��ƍ���̍��W�̍�(�ړ���)��Ԃ��܂�
		*/
		const D3DXVECTOR2 GetMouseVelocity(void)const	{ return mMouseVelocity; }
		
		/**
		* @brief �z�C�[�������ɉ���0�𒴂��鐔���A��O�Ȃ�0�����̐�����Ԃ��܂�
		*/
		constexpr int GetMouseWheel(void)const			{ return mMouseWheel; }
	
		/**
		* @brief �}�E�X�J�[�\���𒆐S�ɌŒ肷�邩
		* @param �Œ肷�邩�̃t���O
		*/
		void SetDoSetCursor(bool aDoSetCursor)			{ mDoSetCursor = aDoSetCursor; }

	private:
		/**
		* @brief �}�E�X�̍��W���X�V���܂�
		*/
		void UpdateMousePosition(void);

		HWND mpHwnd;

		std::array<BYTE, 256> mDownKeyList;
		std::array<BYTE, 256> mPreviousKeyList;

		D3DXVECTOR2 mMousePosition;
		D3DXVECTOR2 mPreviousMousePosition;
		D3DXVECTOR2 mMouseVelocity;

		int		mMouseWheel;
		bool	mChangeWheel;
		//�J�[�\���������ɃZ�b�g���ꂽ��
		bool	mSetCursor;

		//�J�[�\���𒆉��ɃZ�b�g���邩�ۂ�
		bool	mDoSetCursor;

	protected:
		Input(void);
		virtual ~Input(void)override final;
	};
	inline Input& GetInput(void) { return Input::GetInstance(); }
}