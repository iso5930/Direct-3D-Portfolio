#include "stdafx.h"
#include "Include.h"
#include "Loading.h"
#include "Export_Function.h"
#include "SoundMgr.h"

CLoading::CLoading(eLoadingID eLoadID)
: m_pDevice(NULL), m_eLoadID(eLoadID), m_hThread(NULL), m_bComplete(false)
{
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
	ZeroMemory(m_szLoadingMessage, sizeof(TCHAR) * 128);
}

CLoading::~CLoading(void)
{	
	Release();
}

CLoading* CLoading::Create(eLoadingID eLoadID)
{
	CLoading* pLoading = new CLoading(eLoadID);

	if(FAILED(pLoading->InitLoading()))
	{
		Engine::Safe_Delete(pLoading);
	}

	return pLoading;
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{
	CLoading* pLoading = (CLoading*)pArg;

	switch(pLoading->m_eLoadID)
	{
	case Load_Stage:
		pLoading->Loading_Stage1();
		break;

	case Load_Stage1:
		pLoading->Loading_Stage2();
		break;

	case Load_Stage2:
		pLoading->Loading_Stage3();
		break;
	}

	return 0;
}

HRESULT CLoading::InitLoading(void)
{
	InitializeCriticalSection(&m_CSKey);

	m_pDevice = Engine::Get_GraphicDev()->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
	NULL_CHECK_RETURN(m_hThread, E_FAIL);

	return S_OK;
}

void CLoading::Loading_Stage1(void)
{
	HRESULT hr = NULL;

// 	lstrcpy(m_szLoadingMessage, L"Navi Mesh Loading...");
// 	//네비 메쉬 로딩
// 	NaviMeshLoading();

	lstrcpy(m_szLoadingMessage, L"Shader Loading...");
	//쉐이더 로딩

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_Mesh", L"../../Reference/Headers/ShaderMesh.hpp");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_Map", L"../../Reference/Headers/ShaderMap.hpp");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_Skill", L"../../Reference/Headers/ShaderSkill.hpp");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_MagicArrow", L"../../Reference/Headers/ShaderMagicArrow.hpp");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_Kadan", L"../../Reference/Headers/ShaderKadan.hpp");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_ManaAmber", L"../../Reference/Headers/ShaderManaAmber.hpp");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_Shadow", L"../../Reference/Headers/ShaderCreateTexture.hpp");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ShaderMgr()->AddShaderFiles(m_pDevice, L"Shader_Blur", L"../../Reference/Headers/ShaderBlur.hpp");
	FAILED_CHECK_RETURN(hr,);
	
	lstrcpy(m_szLoadingMessage, L"Sound Loading...");
	//사운드 로딩 시작..
	CSoundMgr::GetInstance()->LoadSoundFile();
	
	lstrcpy(m_szLoadingMessage, L"Texture Loading...");
	//텍스쳐 로딩 시작...
	hr = Engine::Get_ResourceMgr()->Add2DSprite(m_pDevice, Resource_Static, L"Texture_Magic_UI" , L"../bin/Resource/UI/Skill_UI_%d.png", 4);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->Add2DSprite(m_pDevice, Resource_Static, L"SkillIcon", L"../bin/Resource/UI/Icon%d.png", 4);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Magic_Arrow" , L"../bin/Resource/Effect/MagicArrow/MagicArrow%d.tga", 4);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"CustomColor_UI" , L"../bin/Resource/UI/CustomColor_UI.png", 1);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Stage, Texture_Cube, L"Texture_SkyBox", L"../bin/Resource/SkyBox%d.dds", 2);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Fire", L"../bin/Resource/Effect/Fire/Fire%d.tga", 9);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Fire2", L"../bin/Resource/Effect/Fire2/Fire.png", 1);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"FireBall_Ring", L"../bin/Resource/Effect/FireBall_Ring/FireBall_Ring%d.png", 8);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"FireBall_Bomb", L"../bin/Resource/Effect/FireBall_Bomb/FireBall_Bomb%d.png", 20);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Ice_Dust", L"../bin/Resource/Effect/Dust/Dust%d.png", 6);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Fire_Dust", L"../bin/Resource/Effect/FireDust/FireDust%d.png", 6);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Scythe_Effect", L"../bin/Resource/Effect/Scythe/Scythe%d.tga", 4);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Blink_Effect", L"../bin/Resource/Effect/Blink/Blink%d.png", 7);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"Ice_Bomb", L"../bin/Resource/Effect/Ice_Bomb/Ice_Bomb%d.png", 20);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"DarkMagic", L"../bin/Resource/Effect/DarkMagic/DarkMagic.tga", 1);
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice, Resource_Static, Texture_Normal, L"DarkFire_Bomb", L"../bin/Resource/Effect/Dark_Bomb/DarkFire_Bomb%d.png", 11);
	FAILED_CHECK_RETURN(hr,);

	lstrcpy(m_szLoadingMessage, L"Buffer Loading...");
	//버퍼 로딩 시작...
	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, Resource_Stage, Buffer_CubeTex, L"Buffer_CubeTex");
	FAILED_CHECK_RETURN(hr,);

	lstrcpy(m_szLoadingMessage, L"Mesh Loading...");
	//메쉬 로딩 시작..

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Stage, Mesh_Static, L"1Stage_Map", L"../bin/Resource/Mesh/Map/1Stage/", L"1Stage.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Dynamic, L"Player", L"../bin/Resource/Mesh/Player/", L"Player.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Dynamic, L"IceBlow_Begin", L"../bin/Resource/Mesh/Skill/IceBlow/", L"Ice_Blow_Begin.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Dynamic, L"IceBlow_End", L"../bin/Resource/Mesh/Skill/IceBlow/", L"Ice_Blow_End.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"MagicArrow_Mesh", L"../bin/Resource/Mesh/Skill/MagicArrow/", L"MagicArrow.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"IceBlast_Mesh", L"../bin/Resource/Mesh/Skill/IceBlast/", L"Test.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"ManaAmber", L"../bin/Resource/Mesh/Skill/ManaAmber/", L"ManaAmber.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Dynamic, L"Kadan", L"../bin/Resource/Mesh/Kadan/", L"Kadan.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Dynamic, L"DarkKnight_Staff", L"../bin/Resource/Mesh/DarkKnight_Staff/", L"DarkKnight_Staff.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Dynamic, L"DarkKnight_Scythe", L"../bin/Resource/Mesh/DarkKnight_Scythe/", L"DarkKnight_Scythe.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"DarkHurricane", L"../bin/Resource/Mesh/Skill/DarkHurricane/", L"DarkHurricane.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"Kadan_Spin", L"../bin/Resource/Mesh/KadanEffect/Spin/", L"Spin.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"MagicArrow_Effect", L"../bin/Resource/Mesh/Effect/MagicArrowEffect/", L"MagicArrowEffect.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Dynamic, L"Kadan_Rage", L"../bin/Resource/Mesh/Kadan_Rage/", L"Kadan_Rage.X");
	FAILED_CHECK_RETURN(hr,);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice, Resource_Static, Mesh_Static, L"Dark_Bomb", L"../bin/Resource/Mesh/Effect/DarkBomb/", L"DarkBomb.X");
	FAILED_CHECK_RETURN(hr,);

	lstrcpy(m_szLoadingMessage, L"Press Enter");
	//로딩 끝..

	m_bComplete = true;
}

void CLoading::Loading_Stage2(void)
{

}

void CLoading::Loading_Stage3(void)
{

}

void CLoading::Release(void)
{
	DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
}

void CLoading::NaviMeshLoading(void)
{
	DWORD dwByte = 0;

	HANDLE hFile = CreateFile(L"../bin/Data/NaviData.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		//FAILED_CHECK_MSG(E_FAIL, L"Navi Mesh Failed");
		//실패
		return;
	}

	int iSize = 0;

	ReadFile(hFile, &iSize, sizeof(int), &dwByte, NULL);

	Engine::Get_NaviMgr()->Reserve_Cell(m_pDevice, iSize);

	Engine::Get_NaviMgr()->NaviAllDelete();

	while(true)
	{
		D3DXVECTOR3 vPoint[3];

		ReadFile(hFile, vPoint, sizeof(D3DXVECTOR3) * 3, &dwByte, NULL);

		if(dwByte == 0)
			break;

		Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	}

	Engine::Get_NaviMgr()->Link_Cell();

	CloseHandle(hFile);
}

TCHAR* CLoading::GetLoadingMessage(void)
{
	return m_szLoadingMessage;
}

bool CLoading::GetComplete(void)
{
	return m_bComplete;
}