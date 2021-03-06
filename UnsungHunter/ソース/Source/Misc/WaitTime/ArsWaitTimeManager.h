/**
* @file ArsWaitTimeManager.h
* @brief 待ち時間クラスを制御するクラス
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
		* @brief カウントを更新します
		* @param カウント数
		* @note 経過時間であればデルタタイム
		*/
		void Update(float aCountSec)
		{
			for (auto&& it : mWaitTimeMap) {
				it.second.Count(aCountSec);
			}
		}

		/**
		* @brief 登録したWaitTimeを取得
		* @param 欲しいWaitTimeの名前
		*/
		WaitTime& GetWaitTime(const std::string& aWaitTimeName)
		{
			auto it = mWaitTimeMap.find(aWaitTimeName);

			//一度も登録されていないので登録する(初期値を入れておく).
			if (it == mWaitTimeMap.end()) {
				mWaitTimeMap[aWaitTimeName].SetWaitTime(1.0f);
				return mWaitTimeMap[aWaitTimeName];
			}

			return it->second;
		}

		/**
		* @brief 登録されているかどうかを返します
		* @param 調べたいWaitTimeの名前
		*/
		bool IsRegistered(const std::string& aWaitTimeName)const
		{
			return mWaitTimeMap.find(aWaitTimeName) == mWaitTimeMap.end() ? false : true;
		}

		/**
		* @brief WaitTimeを登録します
		* @param 登録したいWaitTimeの名前
		* @param 待ち時間
		* @note 二度目の登録はされません
		*/
		void Register(const std::string& aWaitTimeName, float aWaitTimeSec)
		{
			if (mWaitTimeMap.find(aWaitTimeName) == mWaitTimeMap.end()) {
				mWaitTimeMap[aWaitTimeName].SetWaitTime(aWaitTimeSec);
			}
		}

		/**
		* @brief WaitTimeを登録します
		* @param 登録したいWaitTimeの名前
		* @param 最小待ち時間
		* @param 最大待ち時間
		* @note 二度目の登録はされません
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