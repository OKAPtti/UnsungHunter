/**
* @file ArsFpsCounter.h
* @brief Fps�l���J�E���g����N���X
*/

#pragma once

namespace Alrescha {

	class FpsCounter {
	public:
		FpsCounter(void);
		~FpsCounter(void);

		//�v���J�n
		/**
		* @brief �v�����J�n���܂�
		*/
		void Start(void);

		/**
		* @brief �v�����I�����܂�
		*/
		void End(void);

		/**
		* @brief �v���������Ԃ�Ԃ��܂�
		*/
		constexpr float GetDeltaTime(void)const { return mDeltaTime; }
		//
		/**
		* @brief FPS��Ԃ��܂�
		* @note �v���������Ԃ�FPS���Z
		*/
		constexpr float GetFps(void)const		{ return mFps; }

	private:
		float			 mDeltaTime;
		float			 mFps;
		//����FPS���v�Z���鎞�ɁA���ʂȌv�Z�����炷����
		float			 mSumDeltaTime;
		//�f���^�^�C���̕��ς𓾂邽�߂Ƀf���^�^�C���𕡐��m�ۂ��Ă���
		std::list<float> mLogListOfDeltaTime;

		LARGE_INTEGER	 mFrequency;
		LARGE_INTEGER	 mStartTime;
	};

}