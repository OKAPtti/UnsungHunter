#include"ArsFpsCounter.h"

namespace Alrescha {

	FpsCounter::FpsCounter(void)
		: mDeltaTime(0.0f)
		, mFps(0.0f)
		, mSumDeltaTime(0.0f)
		, mLogListOfDeltaTime()
		, mFrequency()
		, mStartTime()
	{

	}

	FpsCounter::~FpsCounter(void)
	{

	}

	void FpsCounter::Start(void)
	{
		QueryPerformanceFrequency(&mFrequency);
		QueryPerformanceCounter(&mStartTime);
	}

	void FpsCounter::End(void)
	{
		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);

		//0除算防止
		if (mFrequency.QuadPart != 0) {
			mDeltaTime = CastFloat(endTime.QuadPart - mStartTime.QuadPart) / CastFloat(mFrequency.QuadPart);
		}

		//ブレークポイント等でバグらないように制限をかける
		constexpr float MaxDeltaTime = 0.5f;
		if (mDeltaTime > MaxDeltaTime) { mDeltaTime = MaxDeltaTime; }

		mLogListOfDeltaTime.push_back(mDeltaTime);

		//リストの数が規定値を超えたら、mSumDeltaTimeとリストを調整する
		constexpr UINT MaxLogNum = 60;
		if (mLogListOfDeltaTime.size() > MaxLogNum) {
			mSumDeltaTime -= mLogListOfDeltaTime.front();
			mLogListOfDeltaTime.pop_front();
		}

		mSumDeltaTime += mDeltaTime;

		mFps = 1.0f / (mSumDeltaTime / CastFloat(mLogListOfDeltaTime.size()));
	}

}