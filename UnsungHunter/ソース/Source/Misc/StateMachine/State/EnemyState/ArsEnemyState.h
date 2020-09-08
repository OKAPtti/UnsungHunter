/**
* @file ArsEnemyState.h
* @brief 敵が使うステートに必要なものを集めたクラス
*/

#pragma once
#include "../ArsState.h"

namespace Alrescha {

	class EnemyState :public State
	{
	public:
		EnemyState(int aType);
		virtual ~EnemyState(void);
		/**
		* @brief スタート処理をします
		* @param ステートマシンのポインタ
		* @param ゲームオブジェクト
		* @param シーン
		* @note Update前に呼ばれます
		*/
		virtual void Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene);
	

	protected:
		WPtr<class Enemy> mpEnemy;
	};

}