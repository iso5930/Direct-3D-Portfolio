/*!
 * \file Shader.h
 * \date 2016/07/25 15:26
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

#ifndef Shader_h__
#define Shader_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXEFFECT			m_pEffect;
	LPD3DXBUFFER			m_pErrBuffer;

private:
	DWORD		m_dwRefCnt;

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath);

public:
	HRESULT		Ready_ShaderFile(const TCHAR* pFilePath);
	CShader*	CloneShader(void);

public:
	void BeginShader(const int& iPassIdx = 0);
	void EndShader(void);

public:
	virtual DWORD Release(void);

public:
	LPD3DXEFFECT GetEffectHandle(void){ return m_pEffect; }

private:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);

public:
	virtual ~CShader(void);
};

END

#endif // Shader_h__