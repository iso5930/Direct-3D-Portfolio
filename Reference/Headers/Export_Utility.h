/*!
 * \file Export_Utility.h
 * \date 2016/07/07 21:41
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

#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Management.h"
#include "LightMgr.h"
#include "ShaderMgr.h"
#include "NaviMgr.h"
#include "SkillMgr.h"
#include "ShadowMgr.h"

BEGIN(Engine)

inline CManagement*		Get_Management(void);
inline CLightMgr*		Get_LightMgr(void);
inline CShaderMgr*		Get_ShaderMgr(void);
inline CNaviMgr*		Get_NaviMgr(void);
inline CSkillMgr*		Get_SkillMgr(void);
inline CShadowMgr*		Get_ShadowMgr(void);

#include "Export_Utility.inl"

END

#endif // Export_Utility_h__