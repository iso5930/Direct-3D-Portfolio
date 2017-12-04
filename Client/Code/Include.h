#ifndef Include_h__
#define Include_h__

#include "Engine_Include.h"
#include "Const.h"

extern HWND g_hWnd;
extern bool g_bCustomColor;
extern bool	g_bCollision_OBB;
extern bool	g_bCollision_BigSphere;
extern bool	g_bCollision_Sphere;

//_finddata_t 사용하기 위해서 포함...
#include <io.h>

#include <Windows.h>
#include "../bin/fmod.h"
#include "../bin/fmod.hpp"
#include "../bin/fmod_dsp.h"
#include "../bin/fmod_errors.h"
#pragma comment (lib, "fmodex_vc.lib")

#endif // Include_h__