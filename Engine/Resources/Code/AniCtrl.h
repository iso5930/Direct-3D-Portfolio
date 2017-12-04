/*!
 * \file AniCtrl.h
 * \date 2016/07/11 21:11
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

#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl
{
private:
	LPD3DXANIMATIONCONTROLLER	m_pAniCtrl;
	UINT						m_iMaxAniSet;
	UINT						m_iCurrentTrack;
	UINT						m_iNewTrack;
	DOUBLE						m_dAccTime;
	DOUBLE						m_dPeriod;
	DOUBLE						m_dMagicNum;

private:
	void Relase(void);

public:
	void SetAnimation(UINT iAniIdx);
	void FrameMove(const float& fTimePerSec);

public:
	LPD3DXANIMATIONCONTROLLER* GetAniCtrl(void);

public:
	DOUBLE	GetTrackPosition(void);
	DOUBLE	GetPeriod(void);
	void	SetTrackPos(DOUBLE dPosition);
	void	SetMagicNum(DOUBLE dMagicNum);

public:
	CAniCtrl(void);
	CAniCtrl(const CAniCtrl& rhs);
	~CAniCtrl(void);
};

END

#endif // AniCtrl_h__