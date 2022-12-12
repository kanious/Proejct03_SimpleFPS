#include "CommandFollowCurve.h"
#include "Object.h"
#include "glm/vec3.hpp"


USING(glm)
USING(std)

CommandFollowCurve::CommandFollowCurve()
	: m_pTargetObj(nullptr), m_eEasyType(Ease_End), m_vDest(vec3(0.f)), m_vDir1(vec3(0.f)), m_vDir2(vec3(0.f))
	, m_fMaxSpeed(0.f), m_fCurSpeed(0.f), m_fIn(0.f), m_fOut(0.f), m_fTime(0.f), m_fElaspedTime(0.f)
	, m_fEaseInTime(0.f), m_fEaseOutTime(0.f), m_fEaseInAccel(0.f), m_fEaseOutAccel(0.f), m_bIsFirstStep(true)
{
}

CommandFollowCurve::CommandFollowCurve(EaseType type, Object* target, vec3 destPos, vec3 viaPos, _float fTime, _float in, _float out, _bool isBezier)
	: m_vDir1(vec3(0.f)), m_vDir2(vec3(0.f)), m_fMaxSpeed(0.f), m_fCurSpeed(0.f), m_fElaspedTime(0.f)
	, m_fEaseInTime(0.f), m_fEaseOutTime(0.f), m_fEaseInAccel(0.f), m_fEaseOutAccel(0.f), m_bIsFirstStep(true)
{
	m_eCommType = Command_FollowCurve;
	m_pTargetObj = target;
	m_eEasyType = type;
	m_vDest = destPos;
	m_vViaPoint = viaPos;
	m_fIn = in;
	m_fOut = out;
	m_fTime = fTime;

	m_bIsBezier = isBezier;
}

CommandFollowCurve::~CommandFollowCurve()
{
}

void CommandFollowCurve::Update(const _float& dt)
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

		if (!m_bIsBezier)
		{
			vec3 vDir = m_vDir1;

			if (m_bIsFirstStep)
			{
				if (m_fElaspedTime >= (m_fTime / 2.f) - dt)
				{
					m_bIsFirstStep = false;
					vPos = m_vViaPoint;
					m_pTargetObj->SetPosition(vPos);
					return;
				}
			}
			else 
				vDir = m_vDir2;

			if (m_fElaspedTime < m_fEaseInTime)
			{
				m_fCurSpeed = m_fEaseInAccel * m_fElaspedTime;
				vPos += vDir * m_fCurSpeed * dt;
			}
			else if (m_fElaspedTime >= m_fEaseInTime && m_fElaspedTime < m_fEaseOutTime)
			{
				m_fCurSpeed = m_fMaxSpeed;
				vPos += vDir * m_fCurSpeed * dt;
			}
			else if (m_fElaspedTime >= m_fEaseOutTime && m_fElaspedTime < m_fTime)
			{
				m_fCurSpeed = m_fMaxSpeed - (m_fEaseOutAccel * (m_fElaspedTime - m_fEaseOutTime));
				vPos += vDir * m_fCurSpeed * dt;
			}
		}
		else
		{
			vec3 p0 = m_vOrigin;
			vec3 p1 = m_vViaPoint;
			vec3 p2 = m_vDest;

			_float t = m_fElaspedTime / m_fTime;
			_float t0 = 1.f - t;

			vPos.x = (pow(t0, 2) * p0.x)
				+ (2 * t * t0 * p1.x)
				+ (pow(t, 2) * p2.x);

			vPos.y = (pow(t0, 2) * p0.y)
				+ (2 * t * t0 * p1.y)
				+ (pow(t, 2) * p2.y);

			vPos.z = (pow(t0, 2) * p0.z)
				+ (2 * t * t0 * p1.z)
				+ (pow(t, 2) * p2.z);
		}

		m_pTargetObj->SetPosition(vPos);
	}
}

void CommandFollowCurve::Run()
{
	if (!m_bIsFinished)
		return;

	if (nullptr == m_pTargetObj)
		return;

	m_bIsFinished = false;
	m_bIsControlEnd = true;

	m_pTargetObj->AddCommand(m_eCommType, this);
}

void CommandFollowCurve::SetControlStart()
{
	SettingInformation();

	m_bIsControlEnd = false;
}

void CommandFollowCurve::SetCommandFinish()
{
	m_bIsFinished = true;
}

void CommandFollowCurve::SettingInformation()
{
	m_bIsFirstStep = true;

	vec3 vTargetPos = m_pTargetObj->GetPosition();
	m_vOrigin = vTargetPos;

	if (m_vDest == vTargetPos)
	{
		m_vDir1 = vec3(0.f);
		m_vDir2 = vec3(0.f);
	}
	else
	{
		m_vDir1 = m_vViaPoint - vTargetPos;
		m_vDir2 = m_vDest - m_vViaPoint;
		m_vDir1 = normalize(m_vDir1);
		m_vDir2 = normalize(m_vDir2);
	}

	_float dist = distance(m_vViaPoint, vTargetPos) + distance(m_vDest, m_vViaPoint);

	if (Ease_Time == m_eEasyType)
	{
		m_fEaseInTime = m_fIn;
		m_fEaseOutTime = m_fTime - m_fOut;

		_float tempSpeed = dist / m_fTime;
		_float area = m_fTime * tempSpeed;

		m_fMaxSpeed = (2 * area) / ((2 * m_fTime) - m_fIn - m_fOut);
		m_fEaseInAccel = m_fMaxSpeed / m_fIn;
		m_fEaseOutAccel = m_fMaxSpeed / m_fOut;
	}
	else
	{
		_float inDist = dist * m_fIn * 0.01;
		_float outDist = dist * m_fOut * 0.01;

		m_fMaxSpeed = (dist + inDist + outDist) / m_fTime;
		m_fEaseInTime = 2 / m_fMaxSpeed * inDist;
		m_fEaseOutTime = m_fTime - (2 / m_fMaxSpeed * outDist);

		m_fEaseInAccel = m_fMaxSpeed / m_fEaseInTime;
		m_fEaseOutAccel = m_fMaxSpeed / (2 / m_fMaxSpeed * outDist);
	}

	m_fCurSpeed = 0.f;
	m_fElaspedTime = 0.f;
}
