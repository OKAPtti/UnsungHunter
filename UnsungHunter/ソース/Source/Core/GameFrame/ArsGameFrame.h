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
		* @brief 初期化します
		* @param ウィンドウハンドル
		*/
		void Initialize(HWND apHwnd);

		bool Run(void);

		/**
		* @brief 更新します
		*/
		void Update(void);

		/**
		* @brief ウィンドウハンドルを返します
		*/
		const HWND GetHwnd(void)const { return mpHwnd; }

		/**
		* @brief デルタタイムを返します
		*/
		float GetDeltaTime(void)const;

		/**
		* @brief ゲーム終了できるようにします
		*/
		void CanFinishGame(void)	{ mCanFinishGame = true; }
				
		/**
		* @brief ゲーム終了できないようにします
		*/
		void CanNotFinishGame(void) { mCanFinishGame = false; }
	private:

		//ウィンドウハンドル.
		HWND					mpHwnd;
		UPtr<FpsCounter>		mpFpsCounter;
		UPtr<FpsAdjuster>		mpFpsAdjuster;

		//ゲームを終了できるか.
		bool mCanFinishGame;
	};

}