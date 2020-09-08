/**
* @file ArsLookPlayerState.h
* @brief プレイヤーが視認できるかステートクラス
*/

#pragma once
#include "ArsEnemyState.h"

namespace Alrescha {

	class LookPlayerState :public EnemyState
	{
	public:
		LookPlayerState(void);
		~LookPlayerState(void)override;
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

		//視野の片側の範囲(ラジアン角)
		/**
		* @brief 視野の片側の範囲(ラジアン角)を設定します
		* @param 視野角
		*/
		void SetOneSideView(float aRadianAngle) { mOneSideRadView = aRadianAngle; }
		
		/**
		* @brief 視認できる距離を設定します
		* @param 距離
		*/
		void SetCanLookDistance(float aDistance) { mCanLookDistanceSq = aDistance * aDistance; }

	private:
		float mOneSideRadView;
		float mCanLookDistanceSq;
	};

}