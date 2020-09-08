/**
* @file ArsLerp.h
* @brief Lerp�N���X
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
		* @brief Lerp�̊J�n���̒l���Z�b�g���܂�
		* @param �J�n���̒l
		*/
		void SetStart(const T& aStart) { mStart = aStart; }

		/**
		* @brief Lerp�̏I�����̒l���Z�b�g���܂�
		* @param �I�����̒l
		*/
		void SetEnd(const T& aEnd) { mEnd = aEnd; }

		/**
		* @brief EndCount���Z�b�g���܂�
		* @param �I���J�E���g
		*/
		void SetEndCount(int aEndCount) { mEndCount = aEndCount; }

		/**
		* @brief �J�E���g�����Z���܂�
		* @param�@���Z�������l
		* @note ����������1
		*/
		void AddCount(int aAddCount = 1)
		{
			mNowCount += aAddCount;
			if (mNowCount > mEndCount) { mNowCount = mEndCount; }
		}

		/**
		* @brief �J�E���g�����Z���܂�
		* @param ���Z�������l
		* @note ����������1
		*/
		void RemoveCount(int aRemoveCount = 1)
		{
			mNowCount -= aRemoveCount;
			if (mNowCount < 0) { mNowCount = 0; }
		}

		/**
		* @brief �J�E���g�����Z�b�g���܂�
		*/
		void Reset(void) { mNowCount = 0; }

		/**
		* @brief �J�E���g���ő�ɂ��܂�
		*/
		void MaxCount(void) { mNowCount = mEndCount; }

		/**
		* @brief ���݂̃J�E���g��Ԃ��܂�
		*/
		int GetCount(void)const { return mNowCount; }

		/**
		* @brief �J�E���g��60�ɒB���Ă��邩�Ԃ��܂�
		*/
		bool IsMaxCount(void) { return mNowCount == mEndCount; }
		
		/**
		* @brief Lerp�������l��Ԃ��܂�
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