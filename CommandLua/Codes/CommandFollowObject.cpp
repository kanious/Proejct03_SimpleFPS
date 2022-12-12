#include "CommandFollowObject.h"
#include "Object.h"
#include "glm/vec3.hpp"


USING(glm)
USING(std)

CommandFollowObject::CommandFollowObject()
	: m_pTargetObj(nullptr), m_pFollowObj(nullptr), m_fMaxSpeed(0.f), m_fTime(0.f), m_fDecelerateDist(0.f)
	, m_fStopDist(0.f), m_fElaspedTime(0.f), m_fCurSpeed(0.f), m_fAccel(10.f)
{
}

CommandFollowObject::CommandFollowObject(Object* target, Object* follow, _float maxSpeed, _float fFollowTime, _float decelerateDist, _float stopDist)
	: m_fElaspedTime(0.f), m_fCurSpeed(0.f)
{
	m_eCommType = Command_FollowObject;
	m_pTargetObj = target;
	m_pFollowObj = follow;
	m_fMaxSpeed = maxSpeed;
	m_fAccel = maxSpeed;
	m_fTime = fFollowTime;
	m_fDecelerateDist = decelerateDist;
	m_fStopDist = stopDist;
}

CommandFollowObject::~CommandFollowObject()
{
}

void CommandFollowObject::Update(const _float& dt)
{
	if (!m_bIsControlEnd)
	{
		m_fElaspedTime += dt;

		if (m_fElaspedTime >= m_fTime)
		{
			m_fElaspedTime = 0.f;
			m_fCurSpeed = 0.f;
			m_bIsControlEnd = true;
		}

		vec3 vTargetPos = m_pTargetObj->GetPosition();
		vec3 vFollowPos = m_pFollowObj->GetPosition();

		_float dist = distance(vFollowPos, vTargetPos);
		if (dist <= m_fStopDist)
		{
			m_fCurSpeed = 0.f;
			return;
		}

		vec3 vDir = vFollowPos - vTargetPos;
		vDir = normalize(vDir);

		if (dist <= m_fDecelerateDist && dist > m_fStopDist)
		{
 			m_fCurSpeed -= m_fAccel * dt;
			if (m_fCurSpeed <= 0.f)
				m_fCurSpeed = 0.f;
		}
		else
		{
			m_fCurSpeed += m_fAccel * dt;
			if (m_fCurSpeed > m_fMaxSpeed)
				m_fCurSpeed = m_fMaxSpeed;
		}

		vTargetPos += vDir * m_fCurSpeed;
		m_pTargetObj->SetPosition(vTargetPos);
	}
}

void CommandFollowObject::Run()
{
	if (!m_bIsFinished)
		return;

	if (nullptr == m_pTargetObj)
		return;

	m_bIsFinished = false;
	m_bIsControlEnd = true;

	m_pTargetObj->AddCommand(m_eCommType, this);
}

void CommandFollowObject::SetControlStart()
{
	m_fElaspedTime = 0.f;
	m_fCurSpeed = 0.f;

	m_bIsControlEnd = false;
}

void CommandFollowObject::SetCommandFinish()
{
	m_bIsFinished = true;
}
