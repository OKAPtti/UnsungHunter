/**
* @file ArsWaitTimeManager.h
* @brief ‘Ò‚¿ŠÔƒNƒ‰ƒX‚ğ§Œä‚·‚éƒNƒ‰ƒX
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
		* @brief ƒJƒEƒ“ƒg‚ğXV‚µ‚Ü‚·
		* @param ƒJƒEƒ“ƒg”
		* @note Œo‰ßŠÔ‚Å‚ ‚ê‚Îƒfƒ‹ƒ^ƒ^ƒCƒ€
		*/
		void Update(float aCountSec)
		{
			for (auto&& it : mWaitTimeMap) {
				it.second.Count(aCountSec);
			}
		}

		/**
		* @brief “o˜^‚µ‚½WaitTime‚ğæ“¾
		* @param —~‚µ‚¢WaitTime‚Ì–¼‘O
		*/
		WaitTime& GetWaitTime(const std::string& aWaitTimeName)
		{
			auto it = mWaitTimeMap.find(aWaitTimeName);

			//ˆê“x‚à“o˜^‚³‚ê‚Ä‚¢‚È‚¢‚Ì‚Å“o˜^‚·‚é(‰Šú’l‚ğ“ü‚ê‚Ä‚¨‚­).
			if (it == mWaitTimeMap.end()) {
				mWaitTimeMap[aWaitTimeName].SetWaitTime(1.0f);
				return mWaitTimeMap[aWaitTimeName];
			}

			return it->second;
		}

		/**
		* @brief “o˜^‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©‚ğ•Ô‚µ‚Ü‚·
		* @param ’²‚×‚½‚¢WaitTime‚Ì–¼‘O
		*/
		bool IsRegistered(const std::string& aWaitTimeName)const
		{
			return mWaitTimeMap.find(aWaitTimeName) == mWaitTimeMap.end() ? false : true;
		}

		/**
		* @brief WaitTime‚ğ“o˜^‚µ‚Ü‚·
		* @param “o˜^‚µ‚½‚¢WaitTime‚Ì–¼‘O
		* @param ‘Ò‚¿ŠÔ
		* @note “ñ“x–Ú‚Ì“o˜^‚Í‚³‚ê‚Ü‚¹‚ñ
		*/
		void Register(const std::string& aWaitTimeName, float aWaitTimeSec)
		{
			if (mWaitTimeMap.find(aWaitTimeName) == mWaitTimeMap.end()) {
				mWaitTimeMap[aWaitTimeName].SetWaitTime(aWaitTimeSec);
			}
		}

		/**
		* @brief WaitTime‚ğ“o˜^‚µ‚Ü‚·
		* @param “o˜^‚µ‚½‚¢WaitTime‚Ì–¼‘O
		* @param Å¬‘Ò‚¿ŠÔ
		* @param Å‘å‘Ò‚¿ŠÔ
		* @note “ñ“x–Ú‚Ì“o˜^‚Í‚³‚ê‚Ü‚¹‚ñ
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