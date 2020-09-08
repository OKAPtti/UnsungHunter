#include"ArsInput.h"
#include"../ArsConfiguration.h"
#include "../../Scene/ArsSceneManager.h"
#include "../../Scene/BaseScene/ArsScene.h"

namespace Alrescha {

	Input::Input(void)
		: mpHwnd(nullptr)
		, mDownKeyList()
		, mPreviousKeyList()
		, mMousePosition({ 0.0f,0.0f })
		, mPreviousMousePosition({ 0.0f,0.0f })
		, mMouseVelocity({ 0.0f,0.0f })
		, mMouseWheel(0)
		, mChangeWheel(false)
		, mSetCursor(false)
		, mDoSetCursor(true)
	{
		UpdateMousePosition();
	}

	Input::~Input(void)
	{

	}

	void Input::Initialize(HWND apHwnd)
	{
		mpHwnd = apHwnd;
		UpdateMousePosition();
	}

	void Input::Update(void)
	{
		//前回のダウンリストを保持
		mPreviousKeyList = mDownKeyList;

		//キーが押されているか調べる
		GetKeyboardState(reinterpret_cast<PBYTE>(mDownKeyList.data()));
		//BYTE最上位ビットを調べる
		for (auto&& it : mDownKeyList) { it = (it & 0x80); }

		if (!mChangeWheel) {
			mMouseWheel = 0;
		}
		mChangeWheel = false;

		mPreviousMousePosition = mMousePosition;
		UpdateMousePosition();


		//タイトルシーン以外の場合はマウスを固定する.
		if (GetSceneMgr().GetScene()->GetType() != SceneType::eTitleScene) {
			mMouseVelocity = (mMousePosition - mPreviousMousePosition);

			//マウスが少しでも移動したらカーソルを中心へ移動させる
			if (mMouseVelocity != D3DXVECTOR2({ 0.0f,0.0f })) {
				SetCursorPos(
					(WindowWidth / 2) + WindowPositionX, (WindowHeight / 2) + WindowPositionY
				);
				UpdateMousePosition();
			}
		}
	}

	void Input::SetWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
		case WM_MOUSEWHEEL:
			mMouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
			mChangeWheel = true;

			break;
		}
	}

	void Input::UpdateMousePosition(void)
	{
		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(mpHwnd, &mousePos);
		mMousePosition = D3DXVECTOR2({ CastFloat(mousePos.x),CastFloat(mousePos.y) });
	}

}