/**
* @file ArsCharacter.h
* @brief プレイヤークラス
*/

#pragma once
#include "ArsCharacter.h"
#include "ArsArrowType.h"

namespace Alrescha {
	class ChildGameObject;
	class Bow;
	class Quiver;
	class Player :public Character
	{
	public:
		Player(void);
		~Player(void)override;

		/**
		* @brief デシリアライズ
		* @param Jsonオブジェクト
		*/
		void Deserialize(const json11::Json& aJsonObject)override;
		/**
		* @brief 初期化をします
		*/
		void Initialize(void)override;
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(float aDeltaTime);
		/**
		* @brief 3D描画をします
		*/
		void Draw3D(void);

		//情報取得.
		/**
		* @brief 移動しているかを返します
		*/
		bool GetIsMove(void)const		{ return mIsMove; }
		/**
		* @brief 弓を構えているかを返します
		*/
		bool GetIsBendBow(void)const	{ return mIsBendBow; }
		/**
		* @brief 矢を放っているかを返します
		*/
		bool GetIsShootArrow(void)const { return mIsShootArrow; }
		/**
		* @brief 矢のタイプを返します
		*/
		const ArrowType& GetArrowType(void)const		{ return mSelectedArrowType; }
		/**
		* @brief 炎の矢の数を返します
		*/
		const UPtr<Clamp<int>>& GetNumberOfArrows(void) { return mpNumOfArrowsForFire; }
	private:
		/**
		* @brief 移動(関連)処理します
		*/
		void MoveAction(float aDeltaTime);

		/**
		* @brief 矢の切り替え処理
		*/
		void SelectArrowAction(void);
		
		/**
		* @brief 選択している矢の残数が1以上かどうか
		*/
		bool NumberOfSelectArrows(void);

		/**
		* @brief 攻撃処理.
		*/
		void AttackAction(void);

		/**
		* @brief スタミナを回復させます
		*/
		void StaminaPlus(void);
		/**
		* @brief スタミナを減少させます
		*/
		void StaminaMinus(void);
		/**
		* @brief スタミナに関する処理をします
		*/
		void StaminaManagement(void);

		//子.
		SPtr<ChildGameObject>	mpChild;
		SPtr<Bow>				mpBow;
		SPtr<Quiver>			mpQuiver;

		WaitTimeManager			mWaitTimeManager;

		bool					mIsMove;
		//弓を構えているか.
		bool					mIsBendBow;
		//矢を放とうとしているか.
		bool					mIsShootArrow;

		//今選択している矢の種類.
		ArrowType				mSelectedArrowType;

		//炎の矢の所持数.
		UPtr<Clamp<int>>		mpNumOfArrowsForFire;

		/**
		* @brief 衝突判定をします
		*/
		void OnCollision(void)override;
	};

}