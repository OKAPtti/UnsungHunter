/**
* @file ArsGoToPlayerState.h
* @brief プレイヤーに向かって移動するステートクラス
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class GoToPlayerState :public EnemyState
	{
	public:
		GoToPlayerState(void);
		~GoToPlayerState(void)override;
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

		/**
		* @brief 次の状態に遷移する距離を設定します
		* @param 距離
		*/
		void SetNextStateDistance(float aLength) { mNextStateLength = aLength; }

	private:
		float mNextStateLength;
		D3DXVECTOR3 mPlayerPos;

		/**
		* @brief プレイヤーとの間に炎の壁があるかどうか調べます
		* @note あった場合はtrue
		*/
		bool IsThereFireWallInFront(void);
	};

}