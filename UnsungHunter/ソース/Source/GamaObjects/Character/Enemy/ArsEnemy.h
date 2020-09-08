/**
* @file ArsEnemy.h
* @brief 敵に必要なものを集めたクラス
*/

#pragma once
#include "../ArsCharacter.h"

namespace Alrescha {

	class Enemy :public Character
	{
	public:
		Enemy(int aType);
		virtual ~Enemy(void);

		/**
		* @brief 初期化をします
		*/
		virtual void Initialize(void);
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		virtual void Update(float aDeltaTime);
		/**
		* @brief 3D描画をします
		*/
		virtual void Draw3D(void);

		/**
		* @brief 高さを設定します
		* @param 設定したい高さ
		*/
		void SetHeight(float aHeight) { mHeight = aHeight; }
		/**
		* @brief 高さを返します
		*/
		float GetHeight(void)const { return mHeight; }

		/**
		* @brief ノックバックする力を設定
		* @param ノックバックする方向が格納された3Dベクトル
		* @note KnockBackStateで使用します
		*/
		void SetImpactDirection(const D3DXVECTOR3& aDirection) { mReceivedImpactDirection = aDirection; }
		/**
		* @brief ノックバックする力を返します
		*/
		const D3DXVECTOR3& GetImpactDirection(void)const { return mReceivedImpactDirection; }
	protected:
		UPtr<class StateMachine> mpStateMachine;
		//中心から地面までの距離.
		float mHeight;

		//死んだ時に、徐々に消していく(0.0f～1.0f).
		float mAlphaValue;
	private:
		//攻撃が当たった場所.
		D3DXVECTOR3 mHitPosition;

		//受けた衝撃(攻撃)の方向.
		D3DXVECTOR3 mReceivedImpactDirection;
	};

}