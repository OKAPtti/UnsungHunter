/**
* @file ArsSprite.h
* @brief �X�v���C�g�𐧌䂷��N���X
*/

#pragma once

namespace Alrescha {
	class Texture;
	class Sprite
	{
	public:
		Sprite(void);
		~Sprite(void) { Release(); }

		/**
		* @brief �����������܂�
		*/
		void Initialize(void);
		/**
		* @brief �X�v���C�g��������܂�
		*/
		void Release(void);
	
		/**
		* @brief �`����J�n���܂�
		*/
		bool Begin(void) {
			HRESULT hr = mpSprite->Begin(D3DXSPRITE_ALPHABLEND);
			if (hr == D3D_OK) { return true; }
			return false;
		}
		
		/**
		* @brief �`����I�����܂�
		*/
		void End(void) { mpSprite->End(); }
		
		/**
		* @brief �ϊ��s����Z�b�g���܂�
		* @param �Z�b�g�������ϊ��s��
		*/
		void SetTransform(const D3DXMATRIX& aMatrix) {
			mpSprite->SetTransform(&aMatrix);
		}

		/**
		* @brief �X�v���C�g�`������܂�
		* @param �`�悵�����e�N�X�`��
		* @param �Z�b�g�������ϊ��s��
		* @param �F(�w�薳���Ȃ甒)
		*/
		void DrawTex(const Texture& aTexture,const D3DXMATRIX& aMatrix, D3DCOLOR aColor = -1);
		
		/**
		* @brief �X�v���C�g�`������܂�
		* @param �`�悵�����e�N�X�`��
		* @param �`�悵�����e�N�X�`���͈̔�
		* @param �Z�b�g�������ϊ��s��
		* @param �F(�w�薳���Ȃ甒)
		*/
		void DrawTex(const Texture& aTexture, RECT aRect, const D3DXMATRIX& aMatrix, D3DCOLOR aColor = -1);

	private:
		LPD3DXSPRITE	mpSprite;
		//�����\���p�t�H���g
		LPD3DXFONT		mpFont;
		//���̃t�H���g���
		D3DXFONT_DESC	mFontDesc;
	};

}