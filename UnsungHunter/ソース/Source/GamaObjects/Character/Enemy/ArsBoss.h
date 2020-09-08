/**
* @file ArsBoss.h
* @brief ボスクラス
*/


#pragma once
#include "ArsEnemy.h"

namespace Alrescha {

	class Boss :public Enemy
	{
	public:
		Boss(void);
		~Boss(void)override final;

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

	private:
		/**
		* @brief 衝突判定をします
		*/
		void OnCollision(void);
		/**
		* @brief オオカミをスポーンします
		*/
		void SpawnWolf(void);
		//オオカミを召喚したか.
		bool mIsSpawned;
	};

}