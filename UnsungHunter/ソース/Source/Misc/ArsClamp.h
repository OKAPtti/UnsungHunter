/**
* @file ArsClamp.h
* @brief �C��(���l)�̌^�ɔ͈͂���������N���X
*/

#pragma once

namespace Alrescha {
	template<class T>
	struct Clamp
	{
		Clamp(void)
			: mMaxValue(0)
			, mMinValue(0)
			, mCurrentValue(0)
		{}

		/**
		* @param �ő�l
		* @param �ŏ��l
		* @param �����l
		*/
		Clamp(T aMax = 0, T aMin = 0, T aCurrentValue = 0)
			: mMaxValue(aMax)
			, mMinValue(aMin)
			, mCurrentValue(aCurrentValue)
		{}

		/**
		* @brief ���������܂�
		* @param �ő�l
		* @param �ŏ��l
		* @param �����l
		*/
		void Initialize(T aMax, T aMin, T aCurrentValue) {
			mMaxValue = aMax;
			mMinValue = aMin;
			mCurrentValue = aCurrentValue;
		}

		/**
		* @brief ���݂̒l��Ԃ��܂�
		*/
		const T GetCurrentValue(void)const { return mCurrentValue; }

		/**
		* @brief ���݂̒l��ݒ肵�܂�
		*/
		void SetCurrentValue(T aCurrentValue) { mCurrentValue = aCurrentValue; }

		/**
		* @brief �ő�l��Ԃ��܂�
		*/
		const T GetMaxValue(void)const { return mMaxValue; }

		/**
		* @brief �ő�l��ݒ肵�܂�
		*/
		void SetMaxValue(T aMaxValue) { mMaxValue = aMaxValue; }
		/**
		* @brief �ŏ��l��Ԃ��܂�
		*/
		const T GetMinValue(void)const { return mMinValue; }

		/**
		* @brief �ŏ��l��ݒ肵�܂�
		*/
		void SetMinValue(T aMinValue) { mMinValue = aMinValue; }

		/**
		* @brief �l���ŏ��l�ȉ�����Ԃ��܂�
		* @note �l���ŏ��l�ȉ��Ȃ�true��Ԃ��܂�
		*/
		bool IsEmpty(void)const { return GetCurrentValue() <= GetMinValue(); }

		/**
		* @brief �l���ő�l�ȏォ��Ԃ��܂�
		* @note �l���ő�l�ȏ�Ȃ�true��Ԃ��܂�
		*/
		bool IsFull(void)const { return GetMaxValue() <= GetCurrentValue(); }

		/**
		* @brief �l�����Z���܂�
		* @note �ő�l���l�͍����Ȃ�܂���
		*/
		void Add(T aAddValue) {
			mCurrentValue += aAddValue;
			if (mCurrentValue >= GetMaxValue()) { mCurrentValue = GetMaxValue(); }
		}
		/**
		* @brief �l�����Z���܂�
		* @note �ŏ��l���l�͒Ⴍ�Ȃ�܂���
		*/
		void Remove(T aRemoveValue) {
			mCurrentValue -= aRemoveValue;
			if (mCurrentValue <= GetMinValue()) { mCurrentValue = GetMinValue(); }
		}

		/**
		* @brief �ő�l���㏸���܂�
		* @note  10 �� 15
		*/
		void MaxAdd(T aAddValue) { mMaxValue += aAddValue; }

		/**
		* @brief �ő�l���������܂�
		* @note�@10 �� 5
		*/
		void MaxRemove(T aRemoveValue) { mMaxValue -= aRemoveValue; }
		
		/**
		* @brief �ŏ��l���������܂�
		* @note -10 �� -5
		*/
		void MinAdd(T aAddValue) { mMinValue += aAddValue; }
		
		/**
		* @brief �ŏ��l���㏸���܂�
		* @note -10 �� -15
		*/
		void MinRemove(T aRemoveValue) { mMinValue -= aRemoveValue; }

		/**
		* @brief ���݂̊�����Ԃ��܂�
		*/
		float Rate(void) { return CastFloat(GetCurrentValue()) / CastFloat(GetMaxValue()); }

		/**
		* @brief �l���ő�l�ɂ��܂�
		*/
		void Full(void) { mCurrentValue = mMaxValue; }
	
		/**
		* @brief �l���ŏ��l�ɂ��܂�
		*/
		void Empty(void) { mCurrentValue = mMinValue; }

		/**
		* @brief �l��0�ɂ��܂�
		*/
		void Zero(void) { mCurrentValue = 0.0f; }

	private:
		T mMaxValue;//�ő�l.
		T mMinValue;//�ŏ��l.
		T mCurrentValue;//���݂̒l.
	};

	template<>
	inline void Clamp<int>::Zero(void)
	{
		mCurrentValue = 0;
	}

	template<>
	inline void Clamp<long>::Zero(void)
	{
		mCurrentValue = 0;
	}

	template<>
	inline void Clamp<float>::Zero(void)
	{
		mCurrentValue = 0.0f;
	}

	template<>
	inline void Clamp<double>::Zero(void)
	{
		mCurrentValue = 0.0f;
	}

	template<>
	inline Clamp<int>::Clamp(void)
	{
		mMaxValue = 0;
		mMinValue = 0;
		mCurrentValue = 0;
	}

	template<>
	inline Clamp<long>::Clamp(void)
	{
		mMaxValue = 0;
		mMinValue = 0;
		mCurrentValue = 0;
	}

	template<>
	inline Clamp<float>::Clamp(void)
	{
		mMaxValue = 0.0f;
		mMinValue = 0.0f;
		mCurrentValue = 0.0f;
	}

	template<>
	inline Clamp<double>::Clamp(void)
	{
		mMaxValue = 0.0f;
		mMinValue = 0.0f;
		mCurrentValue = 0.0f;
	}
}