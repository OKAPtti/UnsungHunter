/**
* @file ArsInput.h
* @brief マウスやキーボードを制御するクラス
*/

#pragma once

#include "../ArsSingleton.h"

namespace Alrescha {

	class Input :public Singleton<Input> 
	{
	public:
		friend class Singleton<Input>;

		/**
		* @brief 初期化します
		* @param ウィンドウハンドル
		*/
		void Initialize(HWND apHwnd);

		/**
		* @brief 更新します
		*/
		void Update(void);

		/**
		* @brief ウィンドウにメッセージを送ります
		* @param MSG
		* @param WPARAM
		* @param LPARAM
		*/
		void SetWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		/**
		* @brief キーが押されていたらtrueを返します
		*/
		constexpr bool GetKey(char aKeyCode)const		{ return mDownKeyList[aKeyCode]; }
		/**
		* @brief キーが押された1フレームだけtrueを返します

		*/
		constexpr bool GetKeyDown(char aKeyCode)const	{ return mDownKeyList[aKeyCode] && !mPreviousKeyList[aKeyCode]; }
		
		/**
		* @brief キーが離された1フレームだけtrueを返します
		*/
		constexpr bool GetKeyUp(char aKeyCode)const		{ return !mDownKeyList[aKeyCode] && mPreviousKeyList[aKeyCode]; }
		/**
		* @brief マウスの座標を返します
		*/
		const D3DXVECTOR2 GetMousePosition(void)const	{ return mMousePosition; }
		
		/**
		* @brief マウスの前回と今回の座標の差(移動量)を返します
		*/
		const D3DXVECTOR2 GetMouseVelocity(void)const	{ return mMouseVelocity; }
		
		/**
		* @brief ホイールが奥に回れば0を超える数字、手前なら0未満の数字を返します
		*/
		constexpr int GetMouseWheel(void)const			{ return mMouseWheel; }
	
		/**
		* @brief マウスカーソルを中心に固定するか
		* @param 固定するかのフラグ
		*/
		void SetDoSetCursor(bool aDoSetCursor)			{ mDoSetCursor = aDoSetCursor; }

	private:
		/**
		* @brief マウスの座標を更新します
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
		//カーソルが中央にセットされた時
		bool	mSetCursor;

		//カーソルを中央にセットするか否か
		bool	mDoSetCursor;

	protected:
		Input(void);
		virtual ~Input(void)override final;
	};
	inline Input& GetInput(void) { return Input::GetInstance(); }
}