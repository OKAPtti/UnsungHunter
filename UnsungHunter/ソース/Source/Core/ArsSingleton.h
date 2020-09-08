/**
* @file ArsSingleton.h
* @brief シングルトンクラス
* @note このクラスを継承することでシングルトン化
* @note publicにfriend class Singleton<継承先>;
* @note protectedにコンストラクタ/デストラクタ;
*/

#pragma once

namespace Alrescha {

	template<class T>
	class Singleton {
	public:
		static T& GetInstance(void)
		{
			static T instance;
			return instance;
		}

	private:
		Singleton(const Singleton& aAnother) = delete;
		void operator =(const Singleton& aAnother) = delete;

	protected:
		Singleton(void) {}
		virtual ~Singleton(void) {}
	};
}