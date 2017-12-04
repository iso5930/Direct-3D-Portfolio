/*!
 * \file SoundMgr.h
 * \date 2016/08/25 20:08
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

#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Include.h"

class CSoundMgr
{
public:
	DECLARE_SINGLETON(CSoundMgr);

private:
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;
	FMOD_SYSTEM*	m_pSystem;
	FMOD_CHANNEL*	m_pBGM;
	FMOD_CHANNEL*	m_pSkillSound;
	FMOD_CHANNEL*	m_pPlayerSound;
	FMOD_CHANNEL*	m_pPlayerAttackSound;
	FMOD_CHANNEL*	m_pPlayerMoveSound;

	FMOD_CHANNEL*	m_pKadanSound;
	FMOD_CHANNEL*	m_pKadanSwordSound;
	FMOD_CHANNEL*	m_pKadanEffectSound;

	FMOD_CHANNEL*	m_pDarkFireSound[16];

private:
	FMOD_RESULT		m_Result;		//실행잘되는지 확인하는 변수.
	UINT			m_iVersion;

public:
	void Init(void);
	void LoadSoundFile(void);
	void PlayBGM(TCHAR* pSoundKey);
	void PlaySkillSound(TCHAR* pSoundKey);
	void StopBGM(void);
	void PlayPlayerSound(TCHAR* pSoundKey);
	void PlayPlayerAttackSound(TCHAR* pSoundKey);
	void PlayPlayerMoveSound(TCHAR* pSoundKey);

	void PlayKadanSound(TCHAR* pSoundKey);
	void PlayKadanSwordSound(TCHAR* pSoundKey);
	void PlayKadanEffectSound(TCHAR* pSoundKey);

	void PlayDarkFireSound(TCHAR* pSoundKey, int iIndex);

public:
	map<TCHAR*, FMOD_SOUND*>* GetSoundMap(void) { return &m_mapSound; }

private:
	void ErrorCheck(FMOD_RESULT _result);

private:
	CSoundMgr(void);
	~CSoundMgr(void);
};

#endif // SoundMgr_h__