// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

// C++ RunTime Header Files
#include <cstdlib>		// FOR RANDOM NUMBER GENERATION
#include <algorithm>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stdio.h>
#include <iostream>
#include <string>
using std::wstring;
#include <sstream>
using namespace std;
#include <iomanip>
#include <fstream>
using namespace std;
#include <process.h>

// DirectX
#include <d3d9.h>
#include <d3dx9.h>

#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
#include "Audio.h"
#include <random>
