/**
* @file ArsConfiguration.h
* @brief ウィンドウに関することやファイルパスを定義
*/

#pragma once

namespace Alrescha {
	constexpr char WindowName[] = "UNSUNG HUNTER";
	constexpr int WindowPositionX = 0;
	constexpr int WindowPositionY = 0;
	constexpr int WindowWidth = 1280;
	constexpr int WindowHeight = 720;
	constexpr int WindowHalfWidth = 1280 / 2;
	constexpr int WindowHalfHeight = 720 / 2;
	constexpr bool FullScreen = false;

	constexpr char FixedTexturePath[]	= "../実行/Data/2D/Texture/";
	constexpr char FixedModelPath[]		= "../実行/Data/3D/";
	constexpr char FixedSoundPath[] = "../実行/Data/Sound/";
	constexpr char FixedModelTexturePath[] = "/Texture/";
	constexpr char FixedModelFilePath[] = "/";
}