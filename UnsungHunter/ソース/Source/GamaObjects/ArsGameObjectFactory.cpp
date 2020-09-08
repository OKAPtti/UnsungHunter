#include "ArsGameObjectFactory.h"
#include "ArsAllGameObjectIncludes.h"

namespace Alrescha {
	SPtr<GameObject> CreateGameObject(const std::string& aClassName,int aType)
	{
		if (aClassName == "GameObject") {return std::make_shared<GameObject>(aType);}
		if (aClassName == "Player") {return std::make_shared<Player>();}
		if (aClassName == "Wolf") {return std::make_shared<Wolf>();}
		if (aClassName == "Boss") {return std::make_shared<Boss>();}
		if (aClassName == "Wall") {return std::make_shared<Wall>();}
		return nullptr;
	}
}