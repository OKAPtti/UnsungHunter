#include"ArsFpsAdjuster.h"
#include"ArsFpsCounter.h"

namespace Alrescha {

	FpsAdjuster::FpsAdjuster(int aTargetFps)
		: mTargetFps(aTargetFps)
		, mOneFrameMilliSecond(1000.0f / mTargetFps)
		, mSumTimeMilliSec(0.0f)
		, mWaitCount(1)
		, mpFpsCounter(std::make_unique<FpsCounter>())
	{

	}

	FpsAdjuster::~FpsAdjuster(void)
	{

	}

	void FpsAdjuster::SetTargetFps(int aTargetFps)
	{
		mTargetFps = aTargetFps;
		mOneFrameMilliSecond = (1000.0f / mTargetFps);
		mSumTimeMilliSec = 0.0f;
		mWaitCount = 1;
	}

	void FpsAdjuster::Wait()
	{
		FpsCounter waitCounter;
		waitCounter.Start();

		mpFpsCounter->End();

		//��t���[�����牽�~���b�o�߂�����
		mSumTimeMilliSec += mpFpsCounter->GetDeltaTime() * 1000.0f;

		//�u1F�b * �F����̉񐔁v�Ɓu�F����̍��v�b�v����A�҂����Ԃ��v�Z����
		int waitTime = 0;
		waitTime = CastInt((mOneFrameMilliSecond * mWaitCount) - (mSumTimeMilliSec));

		//�w�肳�ꂽ�t���[���ڎ��_�̗��z�o�ߎ��Ԃ�1�b�Œ�
		if (mWaitCount == mTargetFps) {
			waitTime = CastInt(1000 - (mSumTimeMilliSec));
		}

		//�҂K�v������Α҂�
		if (waitTime > 0) {
			//���
			if (waitTime > 1000) { waitTime = 1000; }

			timeBeginPeriod(1);
			Sleep(waitTime);
			timeEndPeriod(1);
		}

		mpFpsCounter->Start();

		//mTargetFps�񖈂Ɋ�t���[�����X�V
		if (mWaitCount >= mTargetFps) {
			mWaitCount = 0;
			mSumTimeMilliSec = 0.0f;
		}
		mWaitCount++;

		//��t���[�����X�V���ꂽ�^�C�~���O�͍s��Ȃ�
		if (mWaitCount != 1) {
			waitCounter.End();
			mSumTimeMilliSec += waitCounter.GetDeltaTime() * 1000.0f;
		}
	}

}