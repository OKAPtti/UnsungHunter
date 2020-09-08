/**
* @file ArsFpsAdjuster.h
* @brief FPS値を制御するクラス
*/

#pragma once

namespace Alrescha {

	class FpsCounter;

	class FpsAdjuster {
	public:
		FpsAdjuster(int aTargetFps);
		~FpsAdjuster(void);

		/**
		* @brief FPSを設定
		* @param 設定したいFPS値
		*/
		void SetTargetFps(int aTargetFps);
		
		/**
		* @brief 設定されたFPSを返します
		*/
		int GetTargetFps(void) { return mTargetFps; }

		/**
		* @brief 設定されたFPSを超えないように制御します
		*/
		void Wait(void);

	private:
		//設定されたFPS
		int				mTargetFps;
		//1フレームのミリ秒
		float			mOneFrameMilliSecond;
		//基準フレームからの経過時間
		float			mSumTimeMilliSec;
		//Waitを行った回数
		int				mWaitCount;

		UPtr<FpsCounter>mpFpsCounter;
	};

}