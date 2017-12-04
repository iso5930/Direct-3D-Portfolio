/*!
 * \file ShaderMgr.h
 * \date 2016/07/25 15:34
 *
 * \author Jusin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CShaderMgr
{
public:
	DECLARE_SINGLETON(CShaderMgr);

private:
	map<const TCHAR*, CShader*> m_mapShader;

public:
	HRESULT		AddShaderFiles(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pShaderTag, const TCHAR* pFilePath);
	CShader*	Clone_Shader(const TCHAR* pShaderTag);

private:
	CShader* Find_Shader(const TCHAR* pShaderTag);

public:
	void Release(void);

private:
	CShaderMgr(void);
	~CShaderMgr(void);
};

END

#endif // ShaderMgr_h__