/**
* @file ArsChildGameObject.h
* @brief 子クラス
*/

#pragma once
#include "ArsGameObject.h"

namespace Alrescha {

	class ChildGameObject :public GameObject
	{
	public:
		ChildGameObject(int aType);
		virtual ~ChildGameObject(void);

		/**
		* @brief 初期化をします
		* @param モデル名
		*/
		virtual void Initialize(const std::string& aModelName);
		/**
		* @brief 行列を更新
		* @param 親の行列
		* @param デルタタイム
		*/
		virtual void UpdateTransform(const D3DXMATRIX& aParentMatrix, float aDeltaTime) {};
		/**
		* @brief 行列を更新
		* @param 親の行列
		* @param デルタタイム
		*/
		void Update(const D3DXMATRIX& aParentMatrix, float aDeltaTime);
		/**
		* @brief 歩きアニメーションをします
		*/
		virtual void WalkAnimation(void);

		/**
		* @brief 描画をします
		* @param アルファ値
		* @note 引数無しの場合は1.0f
		*/
		virtual void Draw(float aAlphaValue = 1.0f);
		/**
		* @brief 赤色に描画をします
		*/
		virtual void DrawRed(void);

		/**
		* @brief 子を追加します
		* @param 追加したい子リスト
		*/
		void AddChild(SPtr<ChildGameObject> apAddChildList);
		/**
		* @brief 子を追加します
		* @param 追加したい子リスト
		*/
		void AddChild(std::list<SPtr<ChildGameObject>> apAddChildList);

		/**
		* @brief 全ての子を引数のリストに返します
		* @param 子を格納するリスト
		*/
		void GetChildList(std::list<SPtr<ChildGameObject>>& aAnotherList);
		
		/**
		* @brief 指定した子を引数のリストに返します
		* @param 子を格納するリスト
		* @param 取得したいタイプ
		*/
		void GetChild(std::list<SPtr<ChildGameObject>>& aAnotherList, int aType);

		/**
		* @brief 全ての子のアニメーションフレームを0にします
		*/
		void AllAnimeFrameReset(void);
	private:
		//子.
		std::list<SPtr<ChildGameObject>> mChildList;
	protected:
		//アニメーションするときにのフレーム数.
		float							 mAnimeFrame;

	};
}