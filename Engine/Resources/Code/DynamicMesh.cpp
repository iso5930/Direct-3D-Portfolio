#include "DynamicMesh.h"
#include "Loader.h"
#include "AniCtrl.h"

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pDevice)
: CMesh(pDevice), m_pLoader(NULL), m_pRootBoon(NULL),m_pAniCtrl(NULL), m_pHWSkinningTexture(NULL)
{

}

Engine::CDynamicMesh::CDynamicMesh(CONST CDynamicMesh& rhs)
: CMesh(rhs), m_pLoader(rhs.m_pLoader), m_pRootBoon(rhs.m_pRootBoon), m_pHWSkinningTexture(rhs.m_pHWSkinningTexture)
{
	m_pAniCtrl = new CAniCtrl(*(rhs.m_pAniCtrl));

	++m_dwRefCnt;
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{
	Release();
}

Engine::CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pPath, const TCHAR* pFileName)
{
	CDynamicMesh* pMesh = new CDynamicMesh(pDevice);

	if(FAILED(pMesh->LoadMeshFromFile(pPath, pFileName)))
	{
		Safe_Delete(pMesh);
	}

	return pMesh;
}

Engine::CResources* Engine::CDynamicMesh::CloneResource(void)
{
	return new CDynamicMesh(*this);
}

HRESULT Engine::CDynamicMesh::LoadMeshFromFile(const TCHAR* pPath, const TCHAR* pFileName)
{
	TCHAR szFullPath[128] = L"";

	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	m_pLoader	= new CLoader(m_pDevice, pPath);
	m_pAniCtrl	= new CAniCtrl;

	HRESULT hr = D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, m_pDevice, m_pLoader, NULL, &m_pRootBoon, m_pAniCtrl->GetAniCtrl());

	FAILED_CHECK(hr);

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	UpdateFrameMatrix((DERIVED_FRAME*)m_pRootBoon, &matIdentity);
	SetUpFrameMatrixPointer((DERIVED_FRAME*)m_pRootBoon);

	m_pDevice->CreateTexture(64, 64, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, &m_pHWSkinningTexture, NULL);

	return S_OK;
}

void Engine::CDynamicMesh::RenderMesh(LPD3DXEFFECT pEffect, const D3DXMATRIX* pMatWorld)
{
	if(pEffect != NULL)
	{
		pEffect->Begin(NULL, 0);
		//pEffect->BeginPass(0);	

		Find_MeshContainer(pEffect, (DERIVED_FRAME*)m_pRootBoon);

		//pEffect->EndPass();
		pEffect->End();
	}
	else
	{
		m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pDevice->SetTransform(D3DTS_WORLD, pMatWorld);

		Find_MeshContainer(pEffect, (DERIVED_FRAME*)m_pRootBoon);

		m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

DWORD Engine::CDynamicMesh::Release(void)
{
	if(--m_dwRefCnt == 0)
	{
		m_pLoader->DestroyFrame(m_pRootBoon);

		Safe_Delete(m_pLoader);
		Safe_Delete(m_pAniCtrl);
		Safe_Release(m_pHWSkinningTexture);

		return 0;
	}
	else
	{
		Safe_Delete(m_pAniCtrl);
		--m_dwRefCnt;
	}

	return m_dwRefCnt;
}

void Engine::CDynamicMesh::UpdateFrameMatrix(DERIVED_FRAME* pFrame, const D3DXMATRIX* pParentMatrix)
{
	if(pFrame != NULL)
	{
		pFrame->CombinedMatrix = pFrame->TransformationMatrix * (*pParentMatrix);
	}

	if(pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameFirstChild, &pFrame->CombinedMatrix);
	}
	if(pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrix((DERIVED_FRAME*)pFrame->pFrameSibling, pParentMatrix);
	}
}

void Engine::CDynamicMesh::SetUpFrameMatrixPointer(DERIVED_FRAME* pFrame)
{
	if(pFrame == NULL)
	{
		return;
	}

	if(pFrame->pMeshContainer != NULL)
	{
		if(pFrame->pMeshContainer->pSkinInfo == NULL)
		{
			return;
		}

		DWORD dwNumFrame = pFrame->pMeshContainer->pSkinInfo->GetNumBones();

		((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameConbinedMartix = new D3DXMATRIX*[dwNumFrame];
		ZeroMemory(((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameConbinedMartix, sizeof(D3DXMATRIX*) * dwNumFrame);

		for(DWORD i = 0; i < dwNumFrame; ++i)
		{
			((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer)->ppFrameConbinedMartix[i] = &((DERIVED_FRAME*)D3DXFrameFind(m_pRootBoon, pFrame->pMeshContainer->pSkinInfo->GetBoneName(i)))->CombinedMatrix;
		}
	}

	if(pFrame->pFrameFirstChild != NULL)
	{
		SetUpFrameMatrixPointer((DERIVED_FRAME*)pFrame->pFrameFirstChild);
	}
	if(pFrame->pFrameSibling != NULL)
	{
		SetUpFrameMatrixPointer((DERIVED_FRAME*)pFrame->pFrameSibling);
	}
}

void Engine::CDynamicMesh::Find_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_FRAME* pFrame)
{
	if(pFrame == NULL)
	{
		return;
	}

	if(pFrame->pMeshContainer != NULL)
	{
		Render_MeshContainer(pEffect, (DERIVED_MESHCONTAINER*)pFrame->pMeshContainer);
	}

	if(pFrame->pFrameFirstChild != NULL)
	{
		Find_MeshContainer(pEffect, (DERIVED_FRAME*)pFrame->pFrameFirstChild);
	}
	if(pFrame->pFrameSibling != NULL)
	{
		Find_MeshContainer(pEffect, (DERIVED_FRAME*)pFrame->pFrameSibling);
	}
}

void Engine::CDynamicMesh::Render_MeshContainer(LPD3DXEFFECT pEffect, DERIVED_MESHCONTAINER* pMeshContainer)
{
	if(pMeshContainer->pSkinInfo != NULL)
	{
		DWORD dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();

		if(pMeshContainer->pResourlMatrix == NULL)
		{
			pMeshContainer->pResourlMatrix = new D3DXMATRIX[dwNumBones];
		}

		//소프트웨어 스키닝
// 		for(DWORD i = 0; i < dwNumBones; ++i)
// 		{
// 			pMeshContainer->pResourlMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * *(pMeshContainer->ppFrameConbinedMartix[i]);
// 		}

// 		BYTE* pSour;
// 		BYTE* pDest;
// 
// 		pMeshContainer->pOriMesh->LockVertexBuffer(0, (void**)&pSour);
// 		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, (void**)&pDest);
// 
// 		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pResourlMatrix, NULL, pSour, pDest);
// 		//이 함수가 하는 일을 버텍스 셰이더에게 맡기는 것이 하드웨어 스키닝이다.
// 		pMeshContainer->pOriMesh->UnlockVertexBuffer();
// 		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

		for(DWORD i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			if(pEffect == NULL)
			{
				m_pDevice->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
				m_pDevice->SetTexture(0, pMeshContainer->ppTexture[i]);
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
			}	
			else
			{
				int iEffectIndex = 0;

				if(m_eType == Collision_Player)
				{
					if(pMeshContainer->ppTextureCustom[i] != NULL)
					{
						if(pMeshContainer->bAlpha)
						{
							iEffectIndex = 5;
						}
						else
						{
							iEffectIndex = 4;
						}
					}
					else if(pMeshContainer->ppTextureNormal[i] != NULL)
					{
						if(pMeshContainer->bAlpha)
						{
							iEffectIndex = 3;
						}
						else
						{
							iEffectIndex = 2;
						}
					}
					else if(pMeshContainer->bAlpha)
					{
						iEffectIndex = 1;
					}
					else
						iEffectIndex = 0;
				}
				else if(m_eType == Collision_Kadan)
				{
					if(pMeshContainer->ppTextureNormal[i] != NULL)
					{
						if(pMeshContainer->bAlpha)
						{
							iEffectIndex = 3;
						}
						else
							iEffectIndex = 2;
					}
					else
					{
						if(pMeshContainer->bAlpha)
						{
							iEffectIndex = 1;
						}
						else
							iEffectIndex = 0;
					}
				}
			
				pEffect->BeginPass(iEffectIndex);

				LPD3DXBONECOMBINATION pBoneComb = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneCombinationBuf->GetBufferPointer();

				DWORD dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();
				
				for(DWORD j = 0; j < dwNumBones; ++j)
				{
					UINT iIndex = pBoneComb[i].BoneId[j];
					
					if(iIndex <= 300)
					{
						D3DXMatrixMultiply(&pMeshContainer->pResourlMatrix[j], &pMeshContainer->pFrameOffsetMatrix[iIndex], pMeshContainer->ppFrameConbinedMartix[iIndex]);
					}
				}

				D3DLOCKED_RECT LockRect;
				m_pHWSkinningTexture->LockRect(0, &LockRect, NULL, 0);
				memcpy_s(LockRect.pBits, sizeof(D3DXMATRIX) * dwNumBones, pMeshContainer->pResourlMatrix, sizeof(D3DXMATRIX) * dwNumBones);
				m_pHWSkinningTexture->UnlockRect(0);

				pEffect->SetVector("vMtrlDiffus", (D3DXVECTOR4*)&pMeshContainer->pMaterials[i].MatD3D.Diffuse);
				//pEffect->SetVector("vMtrlAmbient", (D3DXVECTOR4*)&pMeshContainer->pMaterials[i].MatD3D.Ambient);
				pEffect->SetVector("vMtrlSpecular", (D3DXVECTOR4*)&pMeshContainer->pMaterials[i].MatD3D.Specular);
				pEffect->SetVector("vMtrlAmbient", &D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));
				
				pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);
				pEffect->SetTexture("g_BumpTexture", pMeshContainer->ppTextureNormal[i]);
				pEffect->SetTexture("g_SpecularTexture", pMeshContainer->ppTextureSpecular[i]);
				pEffect->SetTexture("g_HWSkinningTexture", m_pHWSkinningTexture);

				pEffect->SetInt("g_iInfluenceMax", pMeshContainer->dwInfluenceMax);
				
				if(m_eType == Collision_Player)
				{
					if(iEffectIndex == 5 || iEffectIndex == 4)
					{
						pEffect->SetTexture("g_CustomTexture", pMeshContainer->ppTextureCustom[i]);
						pEffect->SetVector("g_CustomColor_R", &pMeshContainer->vCustomColorR);
						pEffect->SetVector("g_CustomColor_G", &pMeshContainer->vCustomColorG);
						pEffect->SetVector("g_CustomColor_B", &pMeshContainer->vCustomColorB);
					}					
				}

				pEffect->SetFloat("fPower", 5.f);
				
				pEffect->CommitChanges();

				pMeshContainer->MeshData.pMesh->DrawSubset(i);
				
				pEffect->EndPass();
			}			
		}
	}
}

void Engine::CDynamicMesh::SetAnimation(UINT iAniIdx)
{
	m_pAniCtrl->SetAnimation(iAniIdx);
}

void Engine::CDynamicMesh::FrameMove(const float& fTimePerSec)
{
	m_pAniCtrl->FrameMove(fTimePerSec);

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	UpdateFrameMatrix((DERIVED_FRAME*)m_pRootBoon, &matIdentity);
}

DOUBLE Engine::CDynamicMesh::GetTrackPosition(void)
{	
	return m_pAniCtrl->GetTrackPosition();
}

DOUBLE Engine::CDynamicMesh::GetPeriod(void)
{
	return m_pAniCtrl->GetPeriod();
}

void Engine::CDynamicMesh::SetTrackPos(DOUBLE dTrackPosition)
{
	m_pAniCtrl->SetTrackPos(dTrackPosition);
}

const D3DXMATRIX* Engine::CDynamicMesh::FindFrame(const char* pFrameName)
{	
	DERIVED_FRAME* pFrame = (DERIVED_FRAME*)D3DXFrameFind(m_pRootBoon, pFrameName);

	return &pFrame->CombinedMatrix;
}

void Engine::CDynamicMesh::SetMagicNum(DOUBLE dMagicNum)
{
	m_pAniCtrl->SetMagicNum(dMagicNum);
}

void Engine::CDynamicMesh::SetCustomRGB(D3DXVECTOR4 vCustomColor, eCustomColor _eColor, eCustomParts _Parts)
{
	Find_Mesh((DERIVED_FRAME*)m_pRootBoon, vCustomColor, _eColor, _Parts);
}

void Engine::CDynamicMesh::Find_Mesh(DERIVED_FRAME* pFrame, D3DXVECTOR4 vCustomColor, eCustomColor _eColor, eCustomParts _Parts)
{
	if(pFrame == NULL)
	{
		return;
	}

	if(pFrame->pMeshContainer != NULL)
	{
		SetCustomColor((DERIVED_MESHCONTAINER*)pFrame->pMeshContainer, vCustomColor, _eColor, _Parts);
	}
	if(pFrame->pFrameFirstChild != NULL)
	{
		Find_Mesh((DERIVED_FRAME*)pFrame->pFrameFirstChild, vCustomColor, _eColor, _Parts);
	}
	if(pFrame->pFrameSibling != NULL)
	{
		Find_Mesh((DERIVED_FRAME*)pFrame->pFrameSibling, vCustomColor, _eColor, _Parts);
	}
}

void Engine::CDynamicMesh::SetCustomColor(DERIVED_MESHCONTAINER* pFrame, D3DXVECTOR4 vCustomColor, eCustomColor _eColor, eCustomParts _Parts)
{
 	if(pFrame->pSkinInfo != NULL)
 	{
		if(pFrame->CustomParts == _Parts)
		{
			switch(_eColor)
			{
			case CustomColor_R:
				pFrame->vCustomColorR = vCustomColor;
				break;

			case CustomColor_G:
				pFrame->vCustomColorG = vCustomColor;
				break;

			case CustomColor_B:
				pFrame->vCustomColorB = vCustomColor;
				break;
			}
		}
 	}
}

void Engine::CDynamicMesh::SetOwner(eCollisionType _Type)
{
	m_eType = _Type;
}

void Engine::CDynamicMesh::Render_Shadow(LPD3DXEFFECT pEffect)
{
	pEffect->Begin(NULL, NULL);

	Find_ShadowMeshContainer(pEffect, (DERIVED_FRAME*)m_pRootBoon);

	pEffect->End();
}

void Engine::CDynamicMesh::Render_ShadowMeshContainer(LPD3DXEFFECT pEffect, DERIVED_MESHCONTAINER* pMeshContainer)
{
	if(pMeshContainer->pSkinInfo != NULL)
	{
		DWORD dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();

		if(pMeshContainer->pResourlMatrix == NULL)
		{
			pMeshContainer->pResourlMatrix = new D3DXMATRIX[dwNumBones];
		}

		for(DWORD i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			if(pEffect == NULL)
			{
				m_pDevice->SetMaterial(&pMeshContainer->pMaterials[i].MatD3D);
				m_pDevice->SetTexture(0, pMeshContainer->ppTexture[i]);
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
			}	
			else
			{
				pEffect->BeginPass(0);

				LPD3DXBONECOMBINATION pBoneComb = (LPD3DXBONECOMBINATION)pMeshContainer->pBoneCombinationBuf->GetBufferPointer();

				DWORD dwNumBones = pMeshContainer->pSkinInfo->GetNumBones();

				for(DWORD j = 0; j < dwNumBones; ++j)
				{
					UINT iIndex = pBoneComb[i].BoneId[j];

					if(iIndex <= 100)
					{
						D3DXMatrixMultiply(&pMeshContainer->pResourlMatrix[j], &pMeshContainer->pFrameOffsetMatrix[iIndex], pMeshContainer->ppFrameConbinedMartix[iIndex]);
					}
				}

				D3DLOCKED_RECT LockRect;
				m_pHWSkinningTexture->LockRect(0, &LockRect, NULL, 0);
				memcpy_s(LockRect.pBits, sizeof(D3DXMATRIX) * dwNumBones, pMeshContainer->pResourlMatrix, sizeof(D3DXMATRIX) * dwNumBones);
				m_pHWSkinningTexture->UnlockRect(0);
				pEffect->SetTexture("g_HWSkinningTexture", m_pHWSkinningTexture);
				pEffect->SetInt("g_iInfluenceMax", pMeshContainer->dwInfluenceMax);

				pEffect->CommitChanges();

				pMeshContainer->MeshData.pMesh->DrawSubset(i);

				pEffect->EndPass();
			}			
		}
	}
}

void Engine::CDynamicMesh::Find_ShadowMeshContainer(LPD3DXEFFECT pEffect, DERIVED_FRAME* pFrame)
{
	if(pFrame == NULL)
	{
		return;
	}

	if(pFrame->pMeshContainer != NULL)
	{
		Render_ShadowMeshContainer(pEffect, (DERIVED_MESHCONTAINER*)pFrame->pMeshContainer);
	}

	if(pFrame->pFrameFirstChild != NULL)
	{
		Find_ShadowMeshContainer(pEffect, (DERIVED_FRAME*)pFrame->pFrameFirstChild);
	}
	if(pFrame->pFrameSibling != NULL)
	{
		Find_ShadowMeshContainer(pEffect, (DERIVED_FRAME*)pFrame->pFrameSibling);
	}
}