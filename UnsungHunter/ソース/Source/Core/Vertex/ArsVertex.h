/**
* @file ArsVertex.h
* @brief １つの頂点情報をまとめた構造体
*/

#pragma once

namespace Alrescha {

	struct Vertex
	{
		Vertex(void)
			: mPos({0.0f,0.0f,0.0f})
			, mColor(D3DCOLOR_ARGB(255,255,255,255))
			, mUV({0.0f,0.0f})
		{}

		D3DXVECTOR3 mPos;	//頂点の座標.
		D3DCOLOR	mColor;	//色.
		D3DXVECTOR2 mUV;	//使用する画像サイズ(0.0～1.0).
	};

}