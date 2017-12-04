#include "ShaderMgr.h"
#include "Shader.h"

IMPLEMENT_SINGLETON(Engine::CShaderMgr)

Engine::CShaderMgr::CShaderMgr(void)
{

}

Engine::CShaderMgr::~CShaderMgr(void)
{
	Release();
}

HRESULT Engine::CShaderMgr::AddShaderFiles(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pShaderTag, const TCHAR* pFilePath)
{
	CShader*		pShader = Find_Shader(pShaderTag);
	NULL_CHECK_RETURN(!pShader, E_FAIL);

	pShader = CShader::Create(pDevice, pFilePath);
	NULL_CHECK_RETURN(pShader, E_FAIL);

	m_mapShader.insert(map<const TCHAR*, CShader*>::value_type(pShaderTag, pShader));

	return S_OK;
}

Engine::CShader* Engine::CShaderMgr::Clone_Shader(const TCHAR* pShaderTag)
{
	CShader*		pShader = Find_Shader(pShaderTag);

	if(pShader == NULL)
		return NULL;

	return pShader->CloneShader();
}

Engine::CShader* Engine::CShaderMgr::Find_Shader(const TCHAR* pShaderTag)
{
	map<const TCHAR*, CShader*>::iterator iter = find_if(m_mapShader.begin(), m_mapShader.end(), CTagFinder(pShaderTag) );

	if(iter == m_mapShader.end())
		return NULL;

	return iter->second;
}

void Engine::CShaderMgr::Release(void)
{
	map<const TCHAR*, CShader*>::iterator iter = m_mapShader.begin();
	map<const TCHAR*, CShader*>::iterator iter_end = m_mapShader.end();

	DWORD		dwRefCnt = 0;

	for(iter; iter != iter_end; ++iter)
	{
		dwRefCnt = iter->second->Release();

		if(dwRefCnt == 0)
			Safe_Delete(iter->second);
	}

	m_mapShader.clear();
}