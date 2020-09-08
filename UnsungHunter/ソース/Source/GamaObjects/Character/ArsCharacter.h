/**
* @file ArsCharacter.h
* @brief キャラクターに必要なものを集めたクラス
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {
	class Character :public GameObject
	{
	public:
		Character(int aType);
		~Character(void)override;
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
		* @brief ダメージを与えます
		* @param ダメージ量
		* @param 攻撃の名前
		*/
		void Damage(UINT aDamageValue, const std::string& aDamageName);

		/**
		* @brief 回復をします
		* @param 回復量
		*/
		void Cure(UINT aCureValue);
		/**
		* @brief HPを返します
		*/
		const SPtr<Clamp<int>>& GetHp(void)const { return mpHitPoint; }
		/**
		* @brief スタミナを返します
		*/
		const SPtr<Clamp<float>>& GetStamina(void)const { return mpStamina; }
		
		/**
		* @brief Hitフラグをtrueにします
		*/
		void Hit(void) { mIsHit = true; }
		/**
		* @brief Hitフラグをfalseにします
		*/
		void NotIsHit(void) { mIsHit = false; }
		/**
		* @brief Hitフラグを返します
		*/
		bool GetIsHit(void)const { return mIsHit; }

	protected:
		SPtr<class ChildGameObject> mpChild;

		SPtr<Clamp<int>> mpHitPoint;
		SPtr<Clamp<float>> mpStamina;

		//無敵時間.
		WaitTimeManager mInvincibleTime;
		//無敵状態かどうか.
		bool mIsInvincible;

		//攻撃を食らったか.
		bool mIsHit;
	};

}