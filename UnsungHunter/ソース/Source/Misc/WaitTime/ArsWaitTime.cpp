#include "ArsWaitTime.h"

namespace Alrescha {

	WaitTime::WaitTime(void)
		: mCount(0.0f)
		, mMaxWaitTime(0.0f)
		, mMinWaitTime(0.0f)
		, mWaitTime(0.0f)
	{
	}
	WaitTime::WaitTime(float aWaitTime)
		: mCount(0.0f)
		, mMaxWaitTime(aWaitTime)
		, mMinWaitTime(aWaitTime)
		, mWaitTime(0.0f)
	{
		Recount();
	}
	WaitTime::WaitTime(float aMinWaitTime, float aMaxWaitTime)
		: mCount(0.0f)
		, mMaxWaitTime(aMaxWaitTime)
		, mMinWaitTime(aMinWaitTime)
		, mWaitTime(0.0f)
	{
		Recount();
	}
	WaitTime::~WaitTime(void)
	{
	}
	void WaitTime::SetWaitTime(float aWaitTimeSec)
	{
		mMinWaitTime = mMaxWaitTime = aWaitTimeSec;

		Recount();
	}
	void WaitTime::SetWaitTime(float aMinWaitTimeSec, float aMaxWaitTimeSec)
	{
		mMinWaitTime = aMinWaitTimeSec;
		mMaxWaitTime = aMaxWaitTimeSec;

		Recount();
	}
	void WaitTime::Count(float aCountSec)
	{
		if (mCount >= mWaitTime) { return; }

		mCount += aCountSec;

		if (mCount > mWaitTime) { mCount = mWaitTime; }
	}
	bool WaitTime::IsExceed(bool aDoRecount)
	{
		if (mCount >= mWaitTime) {
			if (aDoRecount) { Recount(); }

			return true;
		}

		return false;
	}
	void WaitTime::Recount(void)
	{
		mCount = 0.0f;

		mWaitTime = GetRandomValue(mMinWaitTime, mMaxWaitTime);
	}
	float WaitTime::GetRandomValue(float aMinValue, float aMaxValue) const
	{
		if (aMinValue == aMaxValue) { return aMinValue; }

		static std::mt19937 mt((std::random_device())());
		return static_cast<float>(std::uniform_real_distribution<>(aMinValue, aMaxValue)(mt));
	}
}