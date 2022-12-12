#include "CommandRotSubPart.h"
#include "Object.h"
#include "glm/vec3.hpp"


USING(glm)
USING(std)

CommandRotSubPart::CommandRotSubPart()
	: m_pTargetObj(nullptr), m_fTime(0.f), m_fElaspedTime(0.f)
{
}

CommandRotSubPart::CommandRotSubPart(Object* target, _float fTime)
	: m_fElaspedTime(0.f)
{
	m_eCommType = Command_RotSubPart;
	m_pTargetObj = target;
	m_fTime = fTime;
}

CommandRotSubPart::~CommandRotSubPart()
{
}

void CommandRotSubPart::Update(const _float& dt)
{
	if (!m_bIsControlEnd)
	{
		m_fElaspedTime += dt;
		if (m_fElaspedTime > m_fTime)
		{
			m_pTargetObj->StopRotatingSubPart();
			m_fElaspedTime = 0.f;
			m_bIsControlEnd = true;
		}
	}
}

void CommandRotSubPart::Run()
{
	if (!m_bIsFinished)
		return;

	if (nullptr == m_pTargetObj)
		return;

	m_bIsFinished = false;
	m_bIsControlEnd = true;

	m_pTargetObj->AddCommand(m_eCommType, this);
}

void CommandRotSubPart::SetControlStart()
{
	m_pTargetObj->StartRotatingSubPart();

	m_bIsControlEnd = false;
}

void CommandRotSubPart::SetCommandFinish()
{
	m_bIsFinished = true;
}