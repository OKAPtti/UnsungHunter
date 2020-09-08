/**
* @file ArsFpsCounter.h
* @brief Fps値をカウントするクラス
*/

#pragma once

namespace Alrescha {

	class FpsCounter {
	public:
		FpsCounter(void);
		~FpsCounter(void);

		//計測開始
		/**
		* @brief 計測を開始します
		*/
		void Start(void);

		/**
		* @brief 計測を終了します
		*/
		void End(void);

		/**
		* @brief 計測した時間を返します
		*/
		constexpr float GetDeltaTime(void)const { return mDeltaTime; }
		//
		/**
		* @brief FPSを返します
		* @note 計測した時間をFPS換算
		*/
		constexpr float GetFps(void)const		{ return mFps; }

	private:
		float			 mDeltaTime;
		float			 mFps;
		//平均FPSを計算する時に、無駄な計算を減らすため
		float			 mSumDeltaTime;
		//デルタタイムの平均を得るためにデルタタイムを複数確保しておく
		std::list<float> mLogListOfDeltaTime;

		LARGE_INTEGER	 mFrequency;
		LARGE_INTEGER	 mStartTime;
	};

}