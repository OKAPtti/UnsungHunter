/**
* @file ArsHelper.h
* @brief �^�̏ȗ���g���₷���悤�ɂ����w���p�[�w�b�_
*/

#pragma once

namespace Alrescha {
	// ���S��Release�֐������s����
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
				//���i���ȗ��������p�t�H�[�}���X�ȗ����̃V�[�h�l�ɂ���
				static std::mt19937 mt((std::random_device())());
				return std::uniform_int_distribution<>(aMin, aMax)(mt);
			}

			inline float Value(float aMin, float aMax)
			{
				//���i���ȗ��������p�t�H�[�}���X�ȗ����̃V�[�h�l�ɂ���
				static std::mt19937 mt((std::random_device())());
				return CastFloat(std::uniform_real_distribution<>(aMin, aMax)(mt));
			}
		}
	}

	//Json�t�@�C���̓ǂݍ��݂ƕ���
	inline json11::Json LoadJson(const std::string& filename) {
		//Json�t�@�C�����J��
		std::ifstream ifs(filename);	//��C++�̃t�@�C���ǂݍ���
		if (!ifs) { return false; }
		//������Ƃ��ēǂݍ���
		std::string strJson((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

		//�������Json�t�@�C�������(�p�[�X)����
		std::string err;
		json11::Json jsonObj = json11::Json::parse(strJson, err);
		if (err.size() > 0) { return nullptr; }//���s
		return jsonObj;
	}


	//Json�f�[�^���m���}�[�W����
	//inline : �Ăяo������œW�J�����֐�
	inline json11::Json MergeJson(const json11::Json& src, const json11::Json& overWrite) {
		json11::Json::object out;	//�ԐM�p
		out = src.object_items();	//�㏑�����������R�s�[
		for (auto&& data : overWrite.object_items()) {
			out[data.first] = data.second;
			//data.first:map�̃L�[�փA�N�Z�X
			//data.second:mao�̗v�f�փA�N�Z�X
		}
		return out;
	}
}