/**
* @file ArsAllIncludes.h
* @brief 全てのファイルに適用するヘッダ
*/

#pragma once

#include <Windows.h>
#include<wrl/client.h>

//C++,STL.
#include<iostream>
#include<array>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>
#include<string>
#include<memory>
#include<cstdlib>
#include<algorithm>
#include<random>
#include<fstream>

#pragma comment(lib, "winmm.lib")

//DirectX.
#pragma comment(lib, "DirectX/d3d9.lib")
#pragma comment(lib, "DirectX/d3dx9.lib")

#include<DirectX/d3dx9.h>
#include<DirectX/dsound.h>

//Json.
#include <json/json11.hpp>

//自作,更新頻度が低い.
#include "ArsHelper.h"
#include "Misc/AllMiscIncludes.h"
#include "Core/Vertex/ArsVertex.h"
#include "Core/Math/ArsVector2.h"
#include "Core/Math/ArsVector3.h"
#include "Core/Math/ArsMatrix44.h"