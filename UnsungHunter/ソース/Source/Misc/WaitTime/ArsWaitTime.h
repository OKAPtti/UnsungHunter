/**
* @file ArsWaitTime.h
* @brief 待ち時間クラス
*/

#pragma once

namespace Alrescha {

	class WaitTime
	{
	public:
		WaitTime(void);
		WaitTime(float aWaitTime);
		WaitTime(float aMinWaitTime,float aMaxWaitTime);
		~WaitTime(void);

		/**
		* @brief 経過した時間を返します
		*/
		float GetCount(void)const { return mCount; }
		
		/**
		* @brief 待ち時間を返します
		*/
		float GetWaitTime(void)const { return mWaitTime; }

		/**
		* @brief 待ち時間をセットします
		*/
		void SetWaitTime(float aWaitTimeSec);
		/**
		* @brief 待ち時間をセットします
		* @param 最小待ち時間
		* @param 最大待ち時間
		* @note 二つ目の引数を渡した場合、第一引数との範囲をランダムで設定する
		*/
		void SetWaitTime(float aMinWaitTimeSec, float aMaxWaitTimeSec);

		/**
		* @brief 時間をカウントします
		*/
		void Count(float aCountSec);
		/**
		* @brief セットされた待ち時間を過ぎたかを返します
		* @param カウントをゼロにするか
		* @note 過ぎるとtrueを返す
		*/
		bool IsExceed(bool aDoRecount = false);

		/**
		* @brief 0からカウントを再開します
		*/
		void Recount(void);

		/**
		* @brief 強制的に残り待ち時間を0にします
		*/
		void ForceCountToExceed(void) { mCount = mWaitTime; }
	private:
		/**
		* @brief ランダムで秒数をゲットします
		*/
		float GetRandomValue(float aMinValue, float aMaxValue)const;

		//ランダムで決める、最小待ち時間と最大待ち時間.
		float mMinWaitTime;
		float mMaxWaitTime;

		float mCount;

		float mWaitTime;
	};

}