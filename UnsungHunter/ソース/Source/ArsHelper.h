/**
* @file ArsHelper.h
* @brief 型の省略や使いやすいようにしたヘルパーヘッダ
*/

#pragma once

namespace Alrescha {
	// 安全にRelease関数を実行する
	template<class T>
	void SafeRelease(T*& p) {
		if (p) {
			p->Release();
			p = nullptr;
		}
	}

	template<class T>
	using SPtr = std::shared_ptr<T>;

	template<class T>
	using UPtr = std::unique_ptr<T>;

	template<class T>
	using WPtr = std::weak_ptr<T>;

	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	template<class T>
	inline int CastInt(T aValue)
	{
		return static_cast<int>(aValue);
	}

	template<class T>
	inline long CastLong(T aValue)
	{
		return static_cast<long>(aValue);
	}

	template<class T>
	inline float CastFloat(T aValue)
	{
		return static_cast<float>(aValue);
	}

	template<class T>
	inline double CastDouble(T aValue)
	{
		return static_cast<double>(aValue);
	}

	namespace Get {

		namespace Random {
			inline int Value(int aMin, int aMax)
			{
				//高品質な乱数を高パフォーマンスな乱数のシード値にする
				static std::mt19937 mt((std::random_device())());
				return std::uniform_int_distribution<>(aMin, aMax)(mt);
			}

			inline float Value(float aMin, float aMax)
			{
				//高品質な乱数を高パフォーマンスな乱数のシード値にする
				static std::mt19937 mt((std::random_device())());
				return CastFloat(std::uniform_real_distribution<>(aMin, aMax)(mt));
			}
		}
	}

	//Jsonファイルの読み込みと分割
	inline json11::Json LoadJson(const std::string& filename) {
		//Jsonファイルを開く
		std::ifstream ifs(filename);	//←C++のファイル読み込み
		if (!ifs) { return false; }
		//文字列として読み込む
		std::string strJson((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

		//文字列のJsonファイルを解析(パース)する
		std::string err;
		json11::Json jsonObj = json11::Json::parse(strJson, err);
		if (err.size() > 0) { return nullptr; }//失敗
		return jsonObj;
	}


	//Jsonデータ同士をマージする
	//inline : 呼び出した先で展開される関数
	inline json11::Json MergeJson(const json11::Json& src, const json11::Json& overWrite) {
		json11::Json::object out;	//返信用
		out = src.object_items();	//上書きされる方をコピー
		for (auto&& data : overWrite.object_items()) {
			out[data.first] = data.second;
			//data.first:mapのキーへアクセス
			//data.second:maoの要素へアクセス
		}
		return out;
	}
}