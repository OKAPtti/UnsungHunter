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

		//基準フレームから何ミリ秒経過したか
		mSumTimeMilliSec += mpFpsCounter->GetDeltaTime() * 1000.0f;

		//「1F秒 * 基準Fからの回数」と「基準Fからの合計秒」から、待ち時間を計算する
		int waitTime = 0;
		waitTime = CastInt((mOneFrameMilliSecond * mWaitCount) - (mSumTimeMilliSec));

		//指定されたフレーム目時点の理想経過時間は1秒固定
		if (mWaitCount == mTargetFps) {
			waitTime = CastInt(1000 - (mSumTimeMilliSec));
		}

		//待つ必要があれば待つ
		if (waitTime > 0) {
			//上限
			if (waitTime > 1000) { waitTime = 1000; }

			timeBeginPeriod(1);
			Sleep(waitTime);
			timeEndPeriod(1);
		}

		mpFpsCounter->Start();

		//mTargetFps回毎に基準フレームを更新
		if (mWaitCount >= mTargetFps) {
			mWaitCount = 0;
			mSumTimeMilliSec = 0.0f;
		}
		mWaitCount++;

		//基準フレームが更新されたタイミングは行わない
		if (mWaitCount != 1) {
			waitCounter.End();
			mSumTimeMilliSec += waitCounter.GetDeltaTime() * 1000.0f;
		}
	}

}