/**
* @file ArsGameObjectType.h
* @brief ゲームオブジェクトのタイプの列挙体
*/

#pragma once

namespace Alrescha {

	enum GameObjectType {
		eGameObjectBegin,
		eCamera,

		eGameObject2DBegin,
		eTitle,
		eResult,
		eGameObject2DEnd,

		eGameObject3DBegin,

		eCharacterBegin,
		ePlayer,

		eEnemyBegin,
		eWolf,
		eBoss,
		eEnemyEnd,

		eCharacterEnd,

		eObjectBegin,
		eTerrain,
		eSkyDome,
		eWall,
		eTree,

		eFragmentBegin,
		eFireWallFragment,
		eFireWall,
		eFragmentEnd,

		eObjectEnd,

		eWeaponBegin,
		eBow,
		eQuiver,

		eArrowBegin,
		eNormalArrow,
		eFireArrow,
		eArrowEnd,
		
		eWeaponEnd,

		eChildGameObjectBegin,
		eHead,
		eLeftArm,
		eRightArm,
		eLeftLeg,
		eRightLeg,

		eFrontLeftLeg,
		eFrontRightLeg,
		eHindLeftLeg,
		eHindRightLeg,
		eChildGameObjectEnd,

		eGameObject3DEnd,

		eUserInterface,

		eGameObjectEnd,
		eSize,
	};

	//引数に渡されたオブジェクトがどのタイプに所属しているか.
	//所属していればtrueを返す.
	bool IsGameObject2D(int aGameObjectType);
	bool IsWall(int aGameObjectType);
	bool IsGameObject3D(int aGameObjectType);
	bool IsChildGameObject(int aGameObjectType);
	bool IsUserInterface(int aGameObjectType);
	bool IsCharacter(int aGameObjectType);
	bool IsWeapon(int aGameObjectType);
	bool IsEnemy(int aGameObjectType);
	bool IsEffect(int aGameObjectType);
	bool IsStageObject(int aGameObjectType);
}