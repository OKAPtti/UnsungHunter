/**
* @file ArsEffect.h
* @brief エフェクトクラス
*/

#pragma once
#include "../BaseGameObjects/ArsGameObject.h"

namespace Alrescha {

	class Effect :public GameObject
	{
	public:
		Effect(int aType);
		virtual ~Effect(void)override;

		/**
		* @brief 初期化をします
		*/
		virtual void Initialize(void);

		/**
		* @brief 行列を更新
		* @param デルタタイム
		* @param ログ取得する行列.
		*/
		virtual void UpdateTransform(float aDeltaTime, const D3DXMATRIX& aMatrix);
		/**
		* @brief 3D描画をします
		*/
		virtual void Draw3D(void)override;

		/**
		* @brief ログリストを先頭から順に削除します
		*/
		void RemoveLogList(void);

		/**
		* @brief 頂点座標の設定します
		* @param 設定したい頂点の座標
		* @note 指定しない場合は{ 0.1f,0.1f,0.0f }
		*/
		void SetVertexPos(const D3DXVECTOR3& aPosition = { 0.1f,0.1f,0.0f });
		
		/**
		* @brief 全頂点の色情報を設定します
		* @param 色
		*/
		void SetVertexColor(D3DCOLOR aColor);
		
		/**
		* @brief ログを取得するサイズを設定します
		* @param 指定したいサイズ
		*/
		void SetLogSize(int aSize) { mMaxListSize = aSize; }
	private:
		//ログを格納するリスト.
		std::list<D3DXMATRIX> mLogList;
		//板ポリゴンの頂点情報を格納するリスト.
		std::array<Vertex, 4> mVertexList;

		//取得するログの最大数.
		int mMaxListSize;
	};

}