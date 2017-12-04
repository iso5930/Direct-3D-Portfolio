#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagParticle
	{
		D3DXVECTOR3 vPos;
	}Particle;

	const DWORD ParticleFVF = D3DFVF_XYZ;

	typedef struct tagSwordEffect
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR2	vTexUV;
	}SWORDEFFECT;

	const DWORD SWORDFVF = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNormal;
		D3DXVECTOR2 vTexUV;
	}VTXTEX;

	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vTex;
	}VTXCUBE;

	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagVertexCubeColor
	{
		D3DXVECTOR3 vPos;
		DWORD		dwColor;
	}VTXCOL;

	const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef struct tagIndex16
	{
		WORD			_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD			_1, _2, _3;
	}INDEX32;

	typedef struct tagFrame_Derived : public D3DXFRAME
	{
		D3DXMATRIX CombinedMatrix;
	}DERIVED_FRAME;

	typedef struct tagMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9* ppTexture;
		LPDIRECT3DTEXTURE9* ppTextureNormal;
		LPDIRECT3DTEXTURE9* ppTextureCustom; //¿°»ö¿ë
		LPDIRECT3DTEXTURE9* ppTextureSpecular;
		LPD3DXMESH			pOriMesh;
		D3DXMATRIX*			pFrameOffsetMatrix;
		D3DXMATRIX**		ppFrameConbinedMartix;
		D3DXMATRIX*			pResourlMatrix;
		bool				bAlpha;
		eCustomParts		CustomParts;
		D3DXVECTOR4			vCustomColorR;
		D3DXVECTOR4			vCustomColorG;
		D3DXVECTOR4			vCustomColorB;

		//ÇÏµå¿þ¾î ½ºÅ°´×
		LPD3DXBUFFER		pBoneCombinationBuf;
		D3DXATTRIBUTERANGE* AttributeTable;
		DWORD				dwNumAttributeGroups;
		DWORD				dwInfluenceMax;
	}DERIVED_MESHCONTAINER;

	typedef struct tagOBB
	{
		D3DXVECTOR3 vPoint[8];
		D3DXVECTOR3 vCenter;
		D3DXVECTOR3 vProjAxis[3];
		D3DXVECTOR3 vAxis[3];
	}OBB;

	typedef struct tagTexture
	{
		LPDIRECT3DTEXTURE9	pTexture;
		D3DXIMAGE_INFO		ImgInfo;
	}TEXINFO;
}

#endif // Engine_Struct_h__
