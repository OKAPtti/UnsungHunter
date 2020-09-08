/**
* @file ArsTexture.h
* @brief �e�N�X�`���𐧌䂷��N���X
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
		* @brief �e�N�X�`����Ԃ��܂�
		*/
		const LPDIRECT3DTEXTURE9 GetDirectTexture(void)const { return mpTexture.Get(); }

		/**
		* @brief �e�N�X�`���̕���Ԃ��܂�
		*/
		int GetWidth(void)const { return mWidth; }
		/**
		* @brief �e�N�X�`���̍�����Ԃ��܂�
		*/
		int GetHeight(void)const { return mHeight; }

		/**
		* @brief �e�N�X�`���̕���1/2��Ԃ��܂�
		*/
		int GetHalfWidth(void)const { return mWidth / 2; }
		/**
		* @brief �e�N�X�`���̍�����1/2��Ԃ��܂�
		*/
		int GetHalfHeight(void)const { return mHeight / 2; }

		/**
		* @brief �e�N�X�`���̃T�C�Y��Ԃ��܂�
		*/
		RECT GetRect(void)const { return { 0,0,mWidth,mHeight }; }

		/**
		* @brief �e�N�X�`���̒��S���ŕԂ��܂�
		*/
		D3DXVECTOR3 GetCenter(void)const { return { CastFloat(GetHalfWidth()),CastFloat(GetHalfHeight()),0.0f }; }

	private:
		ComPtr<IDirect3DTexture9> mpTexture;
		int mWidth;
		int mHeight;
	};
}