/**
* @file ArsGameObjectType.h
* @brief �Q�[���I�u�W�F�N�g�̃^�C�v�̗񋓑�
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

	//�����ɓn���ꂽ�I�u�W�F�N�g���ǂ̃^�C�v�ɏ������Ă��邩.
	//�������Ă����true��Ԃ�.
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