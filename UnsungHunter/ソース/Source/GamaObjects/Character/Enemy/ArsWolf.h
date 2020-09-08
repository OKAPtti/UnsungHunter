/**
* @file ArsWolf.h
* @brief オオカミクラス
*/

#pragma once
#include "ArsEnemy.h"

namespace Alrescha {
	class Wolf :public Enemy
	{
	public:
		Wolf(void);
		~Wolf(void)override final;

		/**
		* @brief 初期化をします
		*/
		void Initialize(void)override final;

		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(float aDeltaTime)override final;
		/**
		* @brief 3D描画をします
		*/
		void Draw3D(void)override final;
		/**
		* @brief Stateを回転ステートにします
		* @note ボスから召喚されたときに使用します
		*/
		void ChangeRotateState(void);
	private:
		/**
		* @brief 衝突判定をします
		*/
		void OnCollision(void)override;
	};
}