/**
* @file ArsFireWallFragment.h
* @brief 炎の壁を制御するクラス
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class FireWallFragment :public GameObject
	{
	public:
		FireWallFragment(void);
		~FireWallFragment(void)override;

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
		/**
		* @brief 3D描画をします
		*/
		void Draw3D(void)override;

		/**
		* @brief 板ポリゴンレイ判定をします
		* @param 当たり判定に使用する情報
		* @param 当たり判定の結果を返す
		*/
		bool HitCheckRayPoly(const InputRayPoly& in, OutputRayPoly& out)override;
	private:
		WaitTimeManager mWaitTimeManager;

		/**
		* @brief アニメーションを更新します
		*/
		void UpdateAnimation(void);

		/**
		* @brief 炎の壁を削除します
		*/
		void Delete(void);

		Vertex mVertex[4];

		//ビルーボードなので当たり判定をするときに傾きをなくす.
		D3DXMATRIX mSubMatrix;

		//現在使う画像.
		int mTextureNumber;
	};

}