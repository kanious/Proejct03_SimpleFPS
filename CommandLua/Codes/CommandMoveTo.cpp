#include "CommandMoveTo.h"
#include "Object.h"
#include "glm/vec3.hpp"


USING(glm)
USING(std)

CommandMoveTo::CommandMoveTo()
	: m_pTargetObj(nullptr), m_eEasyType(Ease_End), m_vDest(vec3(0.f)), m_vDir(vec3(0.f))
	, m_fMaxSpeed(0.f), m_fCurSpeed(0.f), m_fIn(0.f), m_fOut(0.f), m_fTime(0.f), m_fElaspedTime(0.f)
	, m_fEaseInTime(0.f), m_fEaseOutTime(0.f), m_fEaseInAccel(0.f), m_fEaseOutAccel(0.f)
{
}

CommandMoveTo::CommandMoveTo(EaseType type, Object* target, vec3 destPos, _float fTime, _float in, _float out)
	: m_vDir(vec3(0.f)), m_fMaxSpeed(0.f), m_fCurSpeed(0.f), m_fElaspedTime(0.f)
	, m_fEaseInTime(0.f), m_fEaseOutTime(0.f), m_fEaseInAccel(0.f), m_fEaseOutAccel(0.f)
{
	m_eCommType = Command_MoveTo;
	m_pTargetObj = target;
	m_eEasyType = type;
	m_vDest = destPos;
	m_fIn = in;
	m_fOut = out;
	m_fTime = fTime;
}

CommandMoveTo::~CommandMoveTo()
{
}

void CommandMoveTo::Update(const _float& dt)
{
	if (!m_bIsControlEnd)
	{
		vec3 vPos = m_pTargetObj->GetPosition();
		m_fElaspedTime += dt;

		if (m_fElaspedTime >= m_fTime || 0.1f >= distance(vPos, m_vDest))
		{
			m_pTargetObj->SetPosition(m_vDest);
			m_bIsControlEnd = true;
			return;
		}

		if (m_fElaspedTime < m_fEaseInTime) 
		{
			m_fCurSpeed = m_fEaseInAccel * m_fElaspedTime;
			vPos += m_vDir * m_fCurSpeed * dt;
		}
		else if (m_fElaspedTime >= m_fEaseInTime && m_fElaspedTime < m_fEaseOutTime)
		{
			m_fCurSpeed = m_fMaxSpeed;
			vPos += m_vDir * m_fCurSpeed * dt;
		}
		else if (m_fElaspedTime >= m_fEaseOutTime && m_fElaspedTime < m_fTime)
		{
			m_fCurSpeed = m_fMaxSpeed - (m_fEaseOutAccel * (m_fElaspedTime - m_fEaseOutTime));
			vPos += m_vDir * m_fCurSpeed * dt;
		}

		m_pTargetObj->SetPosition(vPos);
	}
}

void CommandMoveTo::Run()
{
	if (!m_bIsFinished)
		return;

	if (nullptr == m_pTargetObj)
		return;

	m_bIsFinished = false;
	m_bIsControlEnd = true;

	m_pTargetObj->AddCommand(m_eCommType, this);
}

void CommandMoveTo::SetControlStart()
{
	SettingInformation();

	m_bIsControlEnd = false;
}

void CommandMoveTo::SetCommandFinish()
{
	m_bIsFinished = true;
}

void CommandMoveTo::SettingInformation()
{
	vec3 vTargetPos = m_pTargetObj->GetPosition();

	if (m_vDest == vTargetPos)
		m_vDir = vec3(0.f);
	else
	{
		m_vDir = m_vDest - vTargetPos;
		m_vDir = normalize(m_vDir);
	}

	_float dist = distance(m_vDest, vTargetPos);

	if (Ease_Time == m_eEasyType)
	{
		m_fEaseInTime = m_fIn;
		m_fEaseOutTime = m_fTime - m_fOut;

		_float tempSpeed = dist / m_fTime;
		_float area = m_fTime * tempSpeed;

		m_fMaxSpeed = (2.f * area) / ((2.f * m_fTime) - m_fIn - m_fOut);
		m_fEaseInAccel = m_fMaxSpeed / m_fIn;
		m_fEaseOutAccel = m_fMaxSpeed / m_fOut;
	}
	else
	{
		_float inDist = dist * m_fIn * 0.01f;
		_float outDist = dist * m_fOut * 0.01f;

		m_fMaxSpeed = (dist + inDist + outDist) / m_fTime;
		m_fEaseInTime = 2.f / m_fMaxSpeed * inDist;
		m_fEaseOutTime = m_fTime - (2.f / m_fMaxSpeed * outDist);

		m_fEaseInAccel = m_fMaxSpeed / m_fEaseInTime;
		m_fEaseOutAccel = m_fMaxSpeed / (2.f / m_fMaxSpeed * outDist);
	}

	m_fCurSpeed = 0.f;
	m_fElaspedTime = 0.f;
}
