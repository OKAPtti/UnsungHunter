/**
* @file ArsFireWall.h
* @brief 炎の壁を生成するクラス
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class Texture;
	class FireWall :public GameObject
	{
	public:
		FireWall(void);
		~FireWall(void)override;

		/**
		* @brief 初期化をします
		* @param 表示する位置
		*/
		void Initialize(const D3DXVECTOR3& aPosition);
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(float aDeltaTime)override;

	private:
		WaitTimeManager mDelayTime;

		/**
		* @brief 炎の壁を生成します
		*/
		void CreateFragment(void);

		//何個生成したかを記録.
		int mCreateCount;
		//矢が刺さった位置.
		D3DXVECTOR3 mPosition;
		//前回のFireWallの位置.
		D3DXVECTOR3 mPreviousPosition;
	};

}