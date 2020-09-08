/**
* @file ArsWall.h
* @brief 見えない壁クラス
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Wall :public GameObject
	{
	public:
		Wall(void);
		~Wall(void)override final;
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
		* @brief レイ判定をします
		* @param 当たり判定に使用する情報
		* @param 当たり判定の結果を返す
		*/
		bool HitCheckRay(const InputRay& in, OutputRay& out)override;

	};

}