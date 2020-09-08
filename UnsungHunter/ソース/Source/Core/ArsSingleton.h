/**
* @file ArsSingleton.h
* @brief �V���O���g���N���X
* @note ���̃N���X���p�����邱�ƂŃV���O���g����
* @note public��friend class Singleton<�p����>;
* @note protected�ɃR���X�g���N�^/�f�X�g���N�^;
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