/**
* @file ArsState.h
* @brief ステートに必要なものを集めたクラス
*/

#pragma once

#include "ArsStateType.h"

namespace Alrescha {
	class Scene;
	class State : public std::enable_shared_from_this<State>
	{
	public:
		State(int aType);
		virtual ~State(void);

		/**
		* @brief スタート処理をします
		* @param ステートマシンのポインタ
		* @param ゲームオブジェクト
		* @param シーン
		* @note Update前に呼ばれます
		*/
		virtual void Start(StateMachine* apStateMachine, WPtr<class GameObject> apGameObject, Scene* apScene);
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		virtual void Update(float aDeltaTime) {}

		/**
		* @brief イベント変更処理をします
		* @note ステートが切り替わる前に実行されます
		*/
		virtual void OnChangeEvent(void) {}
		/**
		* @brief ステートを切り替えるフラグを返します
		*/
		bool GetIsNext(void)const { return mIsNext; }
		/**
		* @brief ステートを強制的強制的切り替えるフラグを返します
		*/
		bool GetIsForciblyChange(void) { return mIsForciblyChange; }

		/**
		* @brief 自身のタイプを返します
		*/
		const int GetType(void)const { return mType; }

		/**
		* @brief 自身を(static)キャストします
		* @param タイプ
		* @note このキャストを使うときは引数が決まっている時だけ.
		*/
		template<class T>
		SPtr<T> Cast(int aType);

		/**
		* @brief 強制的にステートを変更させます
		*/
		void SetForciblyChange(void);
	private:
		int mType;
	protected:
		WPtr<class GameObject> mpGameObject;

		StateMachine* mpStateMachine;

		Scene* mpScene;

		//次の状態に行くための条件.
		//条件が満たされていればtrueにする.
		bool mIsNext;
		//強制的に状態を変更するか.
		bool mIsForciblyChange;
	};

	template<class T>
	inline SPtr<T> State::Cast(int aType)
	{
		if (GetType() == aType) {
			return std::static_pointer_cast<T>(shared_from_this());
		}
		return nullptr;
	}

}