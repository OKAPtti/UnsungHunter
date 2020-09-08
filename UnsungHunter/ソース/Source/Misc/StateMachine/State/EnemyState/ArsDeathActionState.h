/**
* @file ArsDeathActionState.h
* @brief 死亡したときに行うステートクラス
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class DeathActionState :public EnemyState
	{
	public:
		DeathActionState(void);
		~DeathActionState(void)override;
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
		/**
		* @brief アニメーションを実行
		*/
		void Animation(void);

		//処理をしたかどうか.
		bool mIsProcessed;
		float mAnimeFrame;
	};

}