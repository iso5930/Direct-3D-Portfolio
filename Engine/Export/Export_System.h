#ifndef Export_System_h__
#define Export_System_h__

#include "GraphicDevice.h"
#include "TimeMgr.h"
#include "FontMgr.h"
#include "InputMgr.h"

BEGIN(Engine)

inline CGraphicDevice*	Get_GraphicDev(void);
inline CTimeMgr*		Get_TimeMgr(void);
inline CFontMgr*		Get_FontMgr(void);
inline CInputMgr*		Get_InputMgr(void);

#include "Export_System.inl"

END

#endif // Export_System_h__
