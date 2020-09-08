/**
* @file ArsTexture.h
* @brief テクスチャを制御するクラス
*/

#pragma once

namespace Alrescha {
	class Texture
	{
	public:
		Texture(ComPtr<IDirect3DTexture9> apTexture,int aWidth,int aHeight)
			: mpTexture(apTexture)
			, mWidth(aWidth)
			, mHeight(aHeight)
		{}
		~Texture(void)
		{}

		/**
		* @brief テクスチャを返します
		*/
		const LPDIRECT3DTEXTURE9 GetDirectTexture(void)const { return mpTexture.Get(); }

		/**
		* @brief テクスチャの幅を返します
		*/
		int GetWidth(void)const { return mWidth; }
		/**
		* @brief テクスチャの高さを返します
		*/
		int GetHeight(void)const { return mHeight; }

		/**
		* @brief テクスチャの幅の1/2を返します
		*/
		int GetHalfWidth(void)const { return mWidth / 2; }
		/**
		* @brief テクスチャの高さの1/2を返します
		*/
		int GetHalfHeight(void)const { return mHeight / 2; }

		/**
		* @brief テクスチャのサイズを返します
		*/
		RECT GetRect(void)const { return { 0,0,mWidth,mHeight }; }

		/**
		* @brief テクスチャの中心をで返します
		*/
		D3DXVECTOR3 GetCenter(void)const { return { CastFloat(GetHalfWidth()),CastFloat(GetHalfHeight()),0.0f }; }

	private:
		ComPtr<IDirect3DTexture9> mpTexture;
		int mWidth;
		int mHeight;
	};
}