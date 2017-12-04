/*!
 * \file Include.h
 * \date 2016/07/08 18:36
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/


#ifndef Include_h__
#define Include_h__

#include <iostream>

using namespace std;

#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <time.h>

//DirectX 를 사용하기 위해서 추가하거나 포함해야 하는 파일들!!!
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

extern	HWND	g_hWnd;

class CToolView;
extern	CToolView* g_pMainView;

#include "Const.h"
#include "Define.h"

#endif // Include_h__
