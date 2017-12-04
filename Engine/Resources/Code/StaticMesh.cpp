#include "StaticMesh.h"

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pDevice)
:CMesh(pDevice), m_pMesh(NULL), m_pSubset(NULL), m_dwSubsetCnt(0), m_ppTexture(NULL), m_pMtrl(NULL), m_ppNormalTexture(NULL)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
:CMesh(rhs), m_pMesh(rhs.m_pMesh), m_pSubset(rhs.m_pSubset),m_dwSubsetCnt(rhs.m_dwSubsetCnt), 
m_ppTexture(rhs.m_ppTexture), m_pMtrl(rhs.m_pMtrl), m_ppNormalTexture(rhs.m_ppNormalTexture),
m_ppSpecularTexture(rhs.m_ppSpecularTexture)
{
	++m_dwRefCnt;
}

Engine::CStaticMesh::~CStaticMesh(void)
{
	//Release();
}

Engine::CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath, const TCHAR* pFileName)
{
	CStaticMesh* pMesh = new CStaticMesh(pDevice);

	if(FAILED(pMesh->LoadMeshFromFile(pFilePath, pFileName)))
	{
		Safe_Delete(pMesh);
	}

	return pMesh;
}

HRESULT Engine::CStaticMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	HRESULT hr = NULL;

	TCHAR szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	hr = D3DXLoadMeshFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice, NULL, &m_pSubset, NULL, &m_dwSubsetCnt, &m_pMesh);
	FAILED_CHECK_MSG(hr, szFullPath);

	D3DXMATERIAL* pSubSet = ((D3DXMATERIAL*)m_pSubset->GetBufferPointer());

	m_pMtrl				= new D3DMATERIAL9[m_dwSubsetCnt];
	m_ppTexture			= new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppNormalTexture	= new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];
	m_ppSpecularTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pMtrl[i] = pSubSet[i].MatD3D;

		TCHAR szFileName[128] = L"";

		lstrcpy(szFullPath, pPath);

		if(pSubSet[i].pTextureFilename == NULL)
		{
			m_ppTexture[i] = NULL;
			m_ppNormalTexture[i] = NULL;

			continue;
		}

		MultiByteToWideChar(CP_ACP, 0, pSubSet[i].pTextureFilename, strlen(pSubSet[i].pTextureFilename), szFileName, 128);
		lstrcat(szFullPath, szFileName);
		hr = D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTexture[i]);
		FAILED_CHECK_RETURN(hr, E_FAIL);

		TCHAR szNormalName[128] = L"";
		TCHAR szSpecularName[128] = L"";

		int iSize = lstrlen(szFileName) - 4;

		for(int j = 0; j < iSize; ++j)
		{
			szNormalName[j] = szFileName[j];
			szSpecularName[j] = szFileName[j];
		}

		lstrcat(szNormalName, L"_Normal.tga");
		lstrcat(szSpecularName, L"_SP.tga");

		lstrcpy(szFullPath, pPath);
		lstrcat(szFullPath, szNormalName);

		if(FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppNormalTexture[i])))
		{
			m_ppNormalTexture[i] = NULL;
		}

		lstrcpy(szFullPath, pPath);
		lstrcat(szFullPath, szSpecularName);

		if(FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppSpecularTexture[i])))
		{
			m_ppSpecularTexture[i] = NULL;
		}
	}

	return S_OK;
}

void Engine::CStaticMesh::RenderMesh(LPD3DXEFFECT pEffect, const D3DXMATRIX* pMatWorld)
{
	if(pEffect == NULL)
	{
		m_pDevice->SetTransform(D3DTS_WORLD, pMatWorld);

	 	for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
	 	{
	 		if(m_ppTexture[i] != NULL)
	 		{
	 			m_pDevice->SetTexture(0, m_ppTexture[i]);
	 			m_pDevice->SetMaterial(&m_pMtrl[i]);
	 			m_pMesh->DrawSubset(i);
	 		}
	 	}
	}
	else
	{
		pEffect->Begin(NULL, 0);

		for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
		{
			int iEffectPass = 0;

			if(m_ppNormalTexture[i] == NULL)
				iEffectPass = 0;
			else
				iEffectPass = 1;

			pEffect->BeginPass(iEffectPass);

			pEffect->SetVector("g_vMtrlDiffus", (D3DXVECTOR4*)&m_pMtrl[i].Diffuse);
			pEffect->SetVector("g_vMtrlAmbient", (D3DXVECTOR4*)&m_pMtrl[i].Ambient);
			pEffect->SetVector("g_vMtrlSpecular", &D3DXVECTOR4(0.5f, 0.5f, 0.5f, 0.5f));
			//pEffect->SetFloat("fPower", m_pMtrl[i].Power);
			pEffect->SetFloat("g_fPower", 10.f);

			pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);
			pEffect->SetTexture("g_BumpTexture", m_ppNormalTexture[i]);
			pEffect->SetTexture("g_SpecularTexture", m_ppSpecularTexture[i]);

			pEffect->CommitChanges();

			m_pMesh->DrawSubset(i);

			pEffect->EndPass();
		}
		pEffect->End();
	}
}

Engine::CResources* Engine::CStaticMesh::CloneResource(void)
{
	return new CStaticMesh(*this);
}

DWORD Engine::CStaticMesh::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		Safe_Delete_Array(m_pMtrl);

		for(DWORD i = 0; i < m_dwSubsetCnt; ++i)
		{
			Safe_Release(m_ppTexture[i]);
			Safe_Release(m_ppNormalTexture[i]);
			Safe_Release(m_ppSpecularTexture[i]);
		}
		Safe_Delete_Array(m_ppNormalTexture);
		Safe_Delete_Array(m_ppTexture);
		Safe_Delete_Array(m_ppSpecularTexture);
		Safe_Release(m_pMesh);
		Safe_Release(m_pSubset);

		return 0;
	}
	else
		--m_dwRefCnt;

	return m_dwRefCnt;
}

LPD3DXMESH Engine::CStaticMesh::GetMesh(void)
{
	return m_pMesh;
}

LPD3DXBUFFER Engine::CStaticMesh::GetSubSet(void)
{
	return m_pSubset;
}
