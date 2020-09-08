/**
* @file ArsLerp.h
* @brief Lerpクラス
*/

#pragma once

namespace Alrescha {

	template<class T>
	class Lerper
	{
	public:
		Lerper(void)
			: mNowCount(60)
			, mEndCount(60)
			, mStart()
			, mEnd()
		{}

		Lerper(const T& aStart,const T& aEnd)
			: mNowCount(60)
			, mEndCount(60)
			, mStart(aStart)
			, mEnd(aEnd)
		{}

		~Lerper(void){}

		/**
		* @brief Lerpの開始時の値をセットします
		* @param 開始時の値
		*/
		void SetStart(const T& aStart) { mStart = aStart; }

		/**
		* @brief Lerpの終了時の値をセットします
		* @param 終了時の値
		*/
		void SetEnd(const T& aEnd) { mEnd = aEnd; }

		/**
		* @brief EndCountをセットします
		* @param 終了カウント
		*/
		void SetEndCount(int aEndCount) { mEndCount = aEndCount; }

		/**
		* @brief カウントを加算します
		* @param　加算したい値
		* @note 引数無しは1
		*/
		void AddCount(int aAddCount = 1)
		{
			mNowCount += aAddCount;
			if (mNowCount > mEndCount) { mNowCount = mEndCount; }
		}

		/**
		* @brief カウントを減算します
		* @param 減算したい値
		* @note 引数無しは1
		*/
		void RemoveCount(int aRemoveCount = 1)
		{
			mNowCount -= aRemoveCount;
			if (mNowCount < 0) { mNowCount = 0; }
		}

		/**
		* @brief カウントをリセットします
		*/
		void Reset(void) { mNowCount = 0; }

		/**
		* @brief カウントを最大にします
		*/
		void MaxCount(void) { mNowCount = mEndCount; }

		/**
		* @brief 現在のカウントを返します
		*/
		int GetCount(void)const { return mNowCount; }

		/**
		* @brief カウントが60に達しているか返します
		*/
		bool IsMaxCount(void) { return mNowCount == mEndCount; }
		
		/**
		* @brief Lerpをした値を返します
		*/
		T Lerp(void) { return mStart + ((mEnd - mStart) * CastFloat(mNowCount) / CastFloat(mEndCount)); }
	private:
		int mNowCount;
		int mEndCount;
		T mStart;
		T mEnd;
	};

	template<>
	inline D3DXVECTOR2 Lerper<D3DXVECTOR2>::Lerp(void)
	{
		const float time = CastFloat(mNowCount) / CastFloat(mEndCount);
		return {
		mStart.x + ((mEnd.x - mStart.x) * time),
		mStart.y + ((mEnd.y - mStart.y) * time)
		};
	}

	template<>
	inline D3DXVECTOR3 Lerper<D3DXVECTOR3>::Lerp(void)
	{
		const float time = CastFloat(mNowCount) / CastFloat(mEndCount);
		return {
		mStart.x + ((mEnd.x - mStart.x) *  time),
		mStart.y + ((mEnd.y - mStart.y) *  time),
		mStart.z + ((mEnd.z - mStart.z) *  time)
		};
	}

}