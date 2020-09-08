/**
* @file ArsGameObjectFactory.h
* @brief ゲームオブジェクトを生成
*/

#pragma once

#include "ArsGameObjectType.h"

#include "../Core/ArsSingleton.h"

namespace Alrescha {
	/**
	* @brief 初期化をします
	* @param 生成するクラス名
	* @param GameObjectクラスを生成する際に区別するためのタイプ
	*/
	SPtr<GameObject> CreateGameObject(const std::string& aClassName,int aType = eSize);

	class ObjectFactory:public Singleton<ObjectFactory>
	{
	public:
		friend class Singleton<ObjectFactory>;
	private:
		//オブジェクトを管理するマップ.
		std::unordered_map < std::string, SPtr<GameObject>> mObjectMap;
	protected:
		ObjectFactory(void){}
		~ObjectFactory(void) {}
	};
}