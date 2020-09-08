/**
* @file ArsWaitTime.h
* @brief �҂����ԃN���X
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
		* @brief �o�߂������Ԃ�Ԃ��܂�
		*/
		float GetCount(void)const { return mCount; }
		
		/**
		* @brief �҂����Ԃ�Ԃ��܂�
		*/
		float GetWaitTime(void)const { return mWaitTime; }

		/**
		* @brief �҂����Ԃ��Z�b�g���܂�
		*/
		void SetWaitTime(float aWaitTimeSec);
		/**
		* @brief �҂����Ԃ��Z�b�g���܂�
		* @param �ŏ��҂�����
		* @param �ő�҂�����
		* @note ��ڂ̈�����n�����ꍇ�A�������Ƃ͈̔͂������_���Őݒ肷��
		*/
		void SetWaitTime(float aMinWaitTimeSec, float aMaxWaitTimeSec);

		/**
		* @brief ���Ԃ��J�E���g���܂�
		*/
		void Count(float aCountSec);
		/**
		* @brief �Z�b�g���ꂽ�҂����Ԃ��߂�������Ԃ��܂�
		* @param �J�E���g���[���ɂ��邩
		* @note �߂����true��Ԃ�
		*/
		bool IsExceed(bool aDoRecount = false);

		/**
		* @brief 0����J�E���g���ĊJ���܂�
		*/
		void Recount(void);

		/**
		* @brief �����I�Ɏc��҂����Ԃ�0�ɂ��܂�
		*/
		void ForceCountToExceed(void) { mCount = mWaitTime; }
	private:
		/**
		* @brief �����_���ŕb�����Q�b�g���܂�
		*/
		float GetRandomValue(float aMinValue, float aMaxValue)const;

		//�����_���Ō��߂�A�ŏ��҂����Ԃƍő�҂�����.
		float mMinWaitTime;
		float mMaxWaitTime;

		float mCount;

		float mWaitTime;
	};

}