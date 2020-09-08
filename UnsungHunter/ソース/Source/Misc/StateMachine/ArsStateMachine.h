/**
* @file ArsStateMachine.h
* @brief ステートマシンクラス
*/

#pragma once

namespace Alrescha {
	class State;
	class StateChanger;
	class GameObject;
	class Scene;
	class StateMachine
	{
	public:
		StateMachine(void);
		~StateMachine(void);
		
		//引数:対象,シーン.
		/**
		* @brief セットアップをします
		* @param ゲームオブジェクト
		* @param シーン
		*/
		void SetUp(WPtr<GameObject> apGameObject, Scene* apScene);

		/**
		* @brief 更新をします
		* @param デルタタイム
		*/
		void Update(float aDeltaTime);

		/**
		* @brief Stateを登録します
		* @param 登録名
		* @param StateChanger
		*/
		void Register(const std::string& aRegisterName,const SPtr<StateChanger> apStateChanger);

		/**
		* @brief 初めの状態をセットします
		* @param 登録された名前
		*/
		void SetStartState(const std::string& aStartRegisterName);


		/**
		* @brief StateTypeを返します
		*/
		const int GetStateType(void)const;

		/**
		* @brief フラグを返します
		* @param フラグ名
		*/
		bool GetFlg(const std::string& aFlgName)const;
		
		/**
		* @brief フラグセットします
		* @param フラグ名
		* @param フラグ
		*/
		void SetFlg(const std::string& aFlgName, bool aFlg) { mFlgMap[aFlgName] = aFlg; }
		
		/**
		* @brief シーンをセットします
		* @param シーン
		*/
		void SetScene(Scene* apScene) { mpScene = apScene; }
	
		/**
		* @brief mIsForciblyChangeをtrueにします
		*/
		void SetIsForciblyChange(void) { mIsForciblyChange = true; }

		/**
		* @brief ステートを変更します
		* @param 変更したいステートの名前
		*/
		void ChangeState(const std::string& aChangeState);
	private:
		WPtr<GameObject> mpGameObject;
		Scene* mpScene;

		std::unordered_map<std::string, SPtr<StateChanger>> mStateMap;

		//ステートから送られるフラグ
		std::unordered_map<std::string, bool> mFlgMap;

		//現在のステート.
		SPtr<StateChanger> mpCurrentState;

		//強制的にステートを変更するか.
		bool mIsForciblyChange;
	};

}