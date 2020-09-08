/**
* @file ArsStateChanger.h
* @brief ステートを変更するクラス
*/

#pragma once

namespace Alrescha {
	class State;
	class Scene;
	class StateChanger
	{
	public:
		StateChanger(void) = default;
		/**
		* @param 登録するステート
		* @param ステート名
		*/
		StateChanger(const SPtr<State> apState, const std::string& aNextStateName);
		StateChanger(const SPtr<State> apState);
		~StateChanger(void);
		/**
		* @brief スタート処理をします
		* @param ステートマシンのポインタ
		* @param ゲームオブジェクト
		* @param シーン
		* @note Update前に呼ばれます
		*/
		virtual void Start(StateMachine*apStateMachine,WPtr<class GameObject> apGameObject,Scene*apScene);
		
		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		virtual void Update(float aDeltaTime);
	
		/**
		* @brief 次の状態に遷移できるかどうかを返します
		*/
		bool CanNextState(void);

		/**
		* @brief 次に遷移するステートの名前を登録します
		* @param ステートの名前
		*/
		void SetNextRegisterName(const std::string& aNextName) { mNextRegisterName = aNextName; }
		
		/**
		* @brief 次に遷移する名前を返します
		*/
		const std::string& GetNextRegisterName(void)const { return mNextRegisterName; }

		/**
		* @brief ステートのタイプを返します
		*/
		const int GetStateType(void)const;

		/**
		* @brief 強制的にステートを変更
		* @note ChangeStateした時のみ実行されます
		*/
		void ForciblyChange(void);
	private:
		SPtr<State> mpState;

		//次に行く状態を登録した名前.
		std::string mNextRegisterName;

	protected:
	};

}