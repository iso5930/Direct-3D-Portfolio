/*!
 * \file VIBuffer.h
 * \date 2016/07/06 23:18
 *
 * \author Jin
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CResources
{
protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	DWORD					m_dwVtxSize;
	DWORD					m_dwVtxCnt;
	DWORD					m_dwVtxFVF;

	LPDIRECT3DINDEXBUFFER9	m_pIB;
	DWORD					m_dwIdxSize;
	DWORD					m_dwTriCnt;
	D3DFORMAT				m_IdxFmt;

public:
	virtual CResources* CloneResource(void)PURE;
	virtual HRESULT		CreateBuffer(void);
	virtual DWORD		Release(void);

public:
	void Render(const D3DXMATRIX* pmatWorld);

public:
	void	GetVtxInfo(void* pVtxInfo);
	void	SetVtxInfo(void* pVtxInfo);
	void	SetIdxInfo(void* pIdxInfo, DWORD* pTriCnt);

public:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer(void);
};

END

#endif // VIBuffer_h__