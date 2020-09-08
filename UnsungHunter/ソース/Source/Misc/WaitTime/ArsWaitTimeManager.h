/**
* @file ArsWaitTimeManager.h
* @brief �҂����ԃN���X�𐧌䂷��N���X
*/

#pragma once

namespace Alrescha {
	class WaitTimeManager
	{
	public:
		WaitTimeManager(void) {}
		~WaitTimeManager(void) {}

		WaitTime& operator[](const std::string& aWaitTimeName)
		{
			return GetWaitTime(aWaitTimeName);
		}

		/**
		* @brief �J�E���g���X�V���܂�
		* @param �J�E���g��
		* @note �o�ߎ��Ԃł���΃f���^�^�C��
		*/
		void Update(float aCountSec)
		{
			for (auto&& it : mWaitTimeMap) {
				it.second.Count(aCountSec);
			}
		}

		/**
		* @brief �o�^����WaitTime���擾
		* @param �~����WaitTime�̖��O
		*/
		WaitTime& GetWaitTime(const std::string& aWaitTimeName)
		{
			auto it = mWaitTimeMap.find(aWaitTimeName);

			//��x���o�^����Ă��Ȃ��̂œo�^����(�����l�����Ă���).
			if (it == mWaitTimeMap.end()) {
				mWaitTimeMap[aWaitTimeName].SetWaitTime(1.0f);
				return mWaitTimeMap[aWaitTimeName];
			}

			return it->second;
		}

		/**
		* @brief �o�^����Ă��邩�ǂ�����Ԃ��܂�
		* @param ���ׂ���WaitTime�̖��O
		*/
		bool IsRegistered(const std::string& aWaitTimeName)const
		{
			return mWaitTimeMap.find(aWaitTimeName) == mWaitTimeMap.end() ? false : true;
		}

		/**
		* @brief WaitTime��o�^���܂�
		* @param �o�^������WaitTime�̖��O
		* @param �҂�����
		* @note ��x�ڂ̓o�^�͂���܂���
		*/
		void Register(const std::string& aWaitTimeName, float aWaitTimeSec)
		{
			if (mWaitTimeMap.find(aWaitTimeName) == mWaitTimeMap.end()) {
				mWaitTimeMap[aWaitTimeName].SetWaitTime(aWaitTimeSec);
			}
		}

		/**
		* @brief WaitTime��o�^���܂�
		* @param �o�^������WaitTime�̖��O
		* @param �ŏ��҂�����
		* @param �ő�҂�����
		* @note ��x�ڂ̓o�^�͂���܂���
		*/
		void Register(const std::string& aWaitTimeName, float aMinWaitTimeSec, float aMaxWaitTimeSec)
		{
			if (mWaitTimeMap.find(aWaitTimeName) == mWaitTimeMap.end()) {
				mWaitTimeMap[aWaitTimeName].SetWaitTime(aMinWaitTimeSec, aMaxWaitTimeSec);
			}
		}

	private:
		std::unordered_map<std::string, WaitTime> mWaitTimeMap;
	};
}