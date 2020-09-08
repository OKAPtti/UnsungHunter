/**
* @file ArsKnockBackState.h
* @brief ノックバックするステートクラス
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class KnockBackState :public EnemyState
	{
	public:
		KnockBackState(void);
		~KnockBackState(void)override;
		/**
		* @brief スタート処理をします
		* @param ステートマシンのポインタ
		* @param ゲームオブジェクト
		* @param シーン
		* @note Update前に呼ばれます
		*/
		void Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene)override;
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(float aDeltaTime)override;
		/**
		* @brief イベント変更処理をします
		* @note ステートが切り替わる前に実行されます
		*/
		void OnChangeEvent(void)override;

	private:
		//移動する力と向き.
		D3DXVECTOR3 mVelocity;

	};

}