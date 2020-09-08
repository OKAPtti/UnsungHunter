/**
* @file ArsClamp.h
* @brief 任意(数値)の型に範囲を持たせるクラス
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
		* @param 最大値
		* @param 最小値
		* @param 初期値
		*/
		Clamp(T aMax = 0, T aMin = 0, T aCurrentValue = 0)
			: mMaxValue(aMax)
			, mMinValue(aMin)
			, mCurrentValue(aCurrentValue)
		{}

		/**
		* @brief 初期化します
		* @param 最大値
		* @param 最小値
		* @param 初期値
		*/
		void Initialize(T aMax, T aMin, T aCurrentValue) {
			mMaxValue = aMax;
			mMinValue = aMin;
			mCurrentValue = aCurrentValue;
		}

		/**
		* @brief 現在の値を返します
		*/
		const T GetCurrentValue(void)const { return mCurrentValue; }

		/**
		* @brief 現在の値を設定します
		*/
		void SetCurrentValue(T aCurrentValue) { mCurrentValue = aCurrentValue; }

		/**
		* @brief 最大値を返します
		*/
		const T GetMaxValue(void)const { return mMaxValue; }

		/**
		* @brief 最大値を設定します
		*/
		void SetMaxValue(T aMaxValue) { mMaxValue = aMaxValue; }
		/**
		* @brief 最小値を返します
		*/
		const T GetMinValue(void)const { return mMinValue; }

		/**
		* @brief 最小値を設定します
		*/
		void SetMinValue(T aMinValue) { mMinValue = aMinValue; }

		/**
		* @brief 値が最小値以下かを返します
		* @note 値が最小値以下ならtrueを返します
		*/
		bool IsEmpty(void)const { return GetCurrentValue() <= GetMinValue(); }

		/**
		* @brief 値が最大値以上かを返します
		* @note 値が最大値以上ならtrueを返します
		*/
		bool IsFull(void)const { return GetMaxValue() <= GetCurrentValue(); }

		/**
		* @brief 値を加算します
		* @note 最大値より値は高くなりません
		*/
		void Add(T aAddValue) {
			mCurrentValue += aAddValue;
			if (mCurrentValue >= GetMaxValue()) { mCurrentValue = GetMaxValue(); }
		}
		/**
		* @brief 値を減算します
		* @note 最小値より値は低くなりません
		*/
		void Remove(T aRemoveValue) {
			mCurrentValue -= aRemoveValue;
			if (mCurrentValue <= GetMinValue()) { mCurrentValue = GetMinValue(); }
		}

		/**
		* @brief 最大値を上昇します
		* @note  10 → 15
		*/
		void MaxAdd(T aAddValue) { mMaxValue += aAddValue; }

		/**
		* @brief 最大値を減少します
		* @note　10 → 5
		*/
		void MaxRemove(T aRemoveValue) { mMaxValue -= aRemoveValue; }
		
		/**
		* @brief 最小値を減少します
		* @note -10 → -5
		*/
		void MinAdd(T aAddValue) { mMinValue += aAddValue; }
		
		/**
		* @brief 最小値を上昇します
		* @note -10 → -15
		*/
		void MinRemove(T aRemoveValue) { mMinValue -= aRemoveValue; }

		/**
		* @brief 現在の割合を返します
		*/
		float Rate(void) { return CastFloat(GetCurrentValue()) / CastFloat(GetMaxValue()); }

		/**
		* @brief 値を最大値にします
		*/
		void Full(void) { mCurrentValue = mMaxValue; }
	
		/**
		* @brief 値を最小値にします
		*/
		void Empty(void) { mCurrentValue = mMinValue; }

		/**
		* @brief 値を0にします
		*/
		void Zero(void) { mCurrentValue = 0.0f; }

	private:
		T mMaxValue;//最大値.
		T mMinValue;//最小値.
		T mCurrentValue;//現在の値.
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