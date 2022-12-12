#include "CommandStay.h"
#include "Object.h"
#include "glm/vec3.hpp"


USING(glm)
USING(std)

CommandStay::CommandStay()
	: m_pTargetObj(nullptr), m_fTime(0.f), m_fElaspedTime(0.f)
{
}

CommandStay::CommandStay(Object* target, _float fTime)
	: m_fElaspedTime(0.f)
{
	m_eCommType = Command_MoveTo;
	m_pTargetObj = target;
	m_fTime = fTime;
}

CommandStay::~CommandStay()
{
}

void CommandStay::Update(const _float& dt)
{
	if (!m_bIsControlEnd)
	{
		m_fElaspedTime += dt;
		if (m_fElaspedTime > m_fTime)
		{
			m_fElaspedTime = 0.f;
			m_bIsControlEnd = true;
		}
	}
}

void CommandStay::Run()
{
	if (!m_bIsFinished)
		return;

	if (nullptr == m_pTargetObj)
		return;

	m_bIsFinished = false;
	m_bIsControlEnd = true;

	m_pTargetObj->AddCommand(m_eCommType, this);
}

void CommandStay::SetControlStart()
{
	m_bIsControlEnd = false;
}

void CommandStay::SetCommandFinish()
{
	m_bIsFinished = true;
}