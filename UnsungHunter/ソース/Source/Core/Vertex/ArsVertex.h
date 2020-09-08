/**
* @file ArsVertex.h
* @brief �P�̒��_�����܂Ƃ߂��\����
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

		D3DXVECTOR3 mPos;	//���_�̍��W.
		D3DCOLOR	mColor;	//�F.
		D3DXVECTOR2 mUV;	//�g�p����摜�T�C�Y(0.0�`1.0).
	};

}