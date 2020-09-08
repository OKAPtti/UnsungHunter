/**
* @file ArsStateType.h
* @brief ステートのタイプの列挙体
*/

#pragma once

enum StateType {
	eFreeMoveState,
	eLookPlayerState,
	eRotatePlayerState,
	eGoToPlayerState,
	eAttackPlayerState,
	eKnockBackState,
	eStopMoveState,
	eDeathAcitionState,
};