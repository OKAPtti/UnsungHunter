/**
* @file ArsFpsAdjuster.h
* @brief FPS�l�𐧌䂷��N���X
*/

#pragma once

namespace Alrescha {

	class FpsCounter;

	class FpsAdjuster {
	public:
		FpsAdjuster(int aTargetFps);
		~FpsAdjuster(void);

		/**
		* @brief FPS��ݒ�
		* @param �ݒ肵����FPS�l
		*/
		void SetTargetFps(int aTargetFps);
		
		/**
		* @brief �ݒ肳�ꂽFPS��Ԃ��܂�
		*/
		int GetTargetFps(void) { return mTargetFps; }

		/**
		* @brief �ݒ肳�ꂽFPS�𒴂��Ȃ��悤�ɐ��䂵�܂�
		*/
		void Wait(void);

	private:
		//�ݒ肳�ꂽFPS
		int				mTargetFps;
		//1�t���[���̃~���b
		float			mOneFrameMilliSecond;
		//��t���[������̌o�ߎ���
		float			mSumTimeMilliSec;
		//Wait���s������
		int				mWaitCount;

		UPtr<FpsCounter>mpFpsCounter;
	};

}