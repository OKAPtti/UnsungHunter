/**
* @file ArsGameObjectFactory.h
* @brief �Q�[���I�u�W�F�N�g�𐶐�
*/

#pragma once

#include "ArsGameObjectType.h"

#include "../Core/ArsSingleton.h"

namespace Alrescha {
	/**
	* @brief �����������܂�
	* @param ��������N���X��
	* @param GameObject�N���X�𐶐�����ۂɋ�ʂ��邽�߂̃^�C�v
	*/
	SPtr<GameObject> CreateGameObject(const std::string& aClassName,int aType = eSize);

	class ObjectFactory:public Singleton<ObjectFactory>
	{
	public:
		friend class Singleton<ObjectFactory>;
	private:
		//�I�u�W�F�N�g���Ǘ�����}�b�v.
		std::unordered_map < std::string, SPtr<GameObject>> mObjectMap;
	protected:
		ObjectFactory(void){}
		~ObjectFactory(void) {}
	};
}