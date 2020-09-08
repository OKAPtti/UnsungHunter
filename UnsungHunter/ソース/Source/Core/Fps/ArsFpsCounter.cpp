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

		//0���Z�h�~
		if (mFrequency.QuadPart != 0) {
			mDeltaTime = CastFloat(endTime.QuadPart - mStartTime.QuadPart) / CastFloat(mFrequency.QuadPart);
		}

		//�u���[�N�|�C���g���Ńo�O��Ȃ��悤�ɐ�����������
		constexpr float MaxDeltaTime = 0.5f;
		if (mDeltaTime > MaxDeltaTime) { mDeltaTime = MaxDeltaTime; }

		mLogListOfDeltaTime.push_back(mDeltaTime);

		//���X�g�̐����K��l�𒴂�����AmSumDeltaTime�ƃ��X�g�𒲐�����
		constexpr UINT MaxLogNum = 60;
		if (mLogListOfDeltaTime.size() > MaxLogNum) {
			mSumDeltaTime -= mLogListOfDeltaTime.front();
			mLogListOfDeltaTime.pop_front();
		}

		mSumDeltaTime += mDeltaTime;

		mFps = 1.0f / (mSumDeltaTime / CastFloat(mLogListOfDeltaTime.size()));
	}

}