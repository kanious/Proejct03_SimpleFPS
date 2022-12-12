#include "CommandPlaySound.h"
#include "Object.h"
#include "glm/vec3.hpp"
#include "SoundMaster.h"


USING(Engine)
USING(glm)
USING(std)

CommandPlaySound::CommandPlaySound()
	: m_soundTag("")
{
}

CommandPlaySound::CommandPlaySound(std::string soundTag)
{
	m_eCommType = Command_PlaySound;
	m_soundTag = soundTag;
}

CommandPlaySound::~CommandPlaySound()
{
}

void CommandPlaySound::Update(const _float& dt)
{
	if (!m_bIsFinished)
	{
		CSoundMaster::GetInstance()->PlaySound(m_soundTag);
		m_bIsFinished = true;
	}
}

void CommandPlaySound::Run()
{
	if (!m_bIsFinished)
		return;

	m_bIsFinished = false;
	m_bIsControlEnd = true; 
}

void CommandPlaySound::SetControlStart()
{
	m_bIsControlEnd = false;
}

void CommandPlaySound::SetCommandFinish()
{
	m_bIsFinished = true;
}
