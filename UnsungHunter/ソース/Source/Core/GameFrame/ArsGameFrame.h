/**
* @file ArsGameFrame.h
*/

#pragma once

namespace Alrescha {
	class GameScene;
	class FpsCounter;
	class FpsAdjuster;
	class GameFrame
	{
	public:
		GameFrame(void);
		~GameFrame(void);

		/**
		* @brief ���������܂�
		* @param �E�B���h�E�n���h��
		*/
		void Initialize(HWND apHwnd);

		bool Run(void);

		/**
		* @brief �X�V���܂�
		*/
		void Update(void);

		/**
		* @brief �E�B���h�E�n���h����Ԃ��܂�
		*/
		const HWND GetHwnd(void)const { return mpHwnd; }

		/**
		* @brief �f���^�^�C����Ԃ��܂�
		*/
		float GetDeltaTime(void)const;

		/**
		* @brief �Q�[���I���ł���悤�ɂ��܂�
		*/
		void CanFinishGame(void)	{ mCanFinishGame = true; }
				
		/**
		* @brief �Q�[���I���ł��Ȃ��悤�ɂ��܂�
		*/
		void CanNotFinishGame(void) { mCanFinishGame = false; }
	private:

		//�E�B���h�E�n���h��.
		HWND					mpHwnd;
		UPtr<FpsCounter>		mpFpsCounter;
		UPtr<FpsAdjuster>		mpFpsAdjuster;

		//�Q�[�����I���ł��邩.
		bool mCanFinishGame;
	};

}