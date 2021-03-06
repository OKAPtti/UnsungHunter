#pragma once

#include "ArsGameObjectType.h"

namespace Alrescha {
	bool IsGameObject2D(int aGameObjectType)
	{
		if (GameObjectType::eGameObject2DBegin <= aGameObjectType && aGameObjectType <= GameObjectType::eGameObject2DEnd) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsWall(int aGameObjectType)
	{
		if (GameObjectType::eWall == aGameObjectType || GameObjectType::eFireWall == aGameObjectType) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsGameObject3D(int aGameObjectType)
	{
		if (GameObjectType::eGameObject3DBegin <= aGameObjectType && aGameObjectType <= GameObjectType::eGameObject3DEnd) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsChildGameObject(int aGameObjectType)
	{
		if (GameObjectType::eChildGameObjectBegin <= aGameObjectType && aGameObjectType <= GameObjectType::eChildGameObjectEnd) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsUserInterface(int aGameObjectType)
	{
		if (GameObjectType::eUserInterface == aGameObjectType) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsCharacter(int aGameObjectType)
	{
		if (GameObjectType::eCharacterBegin <= aGameObjectType && aGameObjectType <= GameObjectType::eCharacterEnd) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsWeapon(int aGameObjectType)
	{
		if (GameObjectType::eWeaponBegin <= aGameObjectType && aGameObjectType <= GameObjectType::eWeaponEnd) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsEnemy(int aGameObjectType)
	{
		if (GameObjectType::eEnemyBegin <= aGameObjectType && aGameObjectType <= GameObjectType::eEnemyEnd) {
			return true;
		}
		//属さない.
		return false;
	}
	bool IsEffect(int aGameObjectType)
	{
		if (GameObjectType::eFragmentBegin <= aGameObjectType && aGameObjectType <= GameObjectType::eFragmentEnd) {
			return true;
		}
		//属さない.
		return false;
	}

	bool IsStageObject(int aGameObjectType)
	{
		if (eTree == aGameObjectType || eTerrain == aGameObjectType || eSkyDome == aGameObjectType ) {
			return true;
		}
		//属さない.
		return false;
	}
	
}