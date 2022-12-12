#include "CommandOrientTo.h"
#include "Object.h"
#include "glm/vec3.hpp"


USING(glm)
USING(std)

CommandOrientTo::CommandOrientTo()
	: m_pTargetObj(nullptr), m_eEasyType(Ease_End), m_vRotDest(vec3(0.f)), m_vRotDir(vec3(0.f))
	, m_vMaxSpeed(vec3(0.f)), m_vCurSpeed(vec3(0.f)), m_fIn(0.f), m_fOut(0.f), m_fTime(0.f), m_fElaspedTime(0.f)
	, m_vEaseInTime(vec3(0.f)), m_vEaseOutTime(vec3(0.f))
	, m_vEaseInAccel(vec3(0.f)), m_vEaseOutAccel(vec3(0.f))
{
}

CommandOrientTo::CommandOrientTo(EaseType type, Object* target, vec3 rotDest, vec3 rotDir, _float fTime, _float in, _float out)
	: m_vMaxSpeed(vec3(0.f)), m_vCurSpeed(vec3(0.f)), m_fElaspedTime(0.f)
	, m_vEaseInTime(vec3(0.f)), m_vEaseOutTime(vec3(0.f)), m_vEaseInAccel(vec3(0.f)), m_vEaseOutAccel(vec3(0.f))
{
	m_eCommType = Command_OrientTo;
	m_pTargetObj = target;
	m_eEasyType = type;
	m_vRotDest = rotDest;
	m_vRotDir = rotDir;
	m_fTime = fTime;
	m_fIn = in;
	m_fOut = out;
}

CommandOrientTo::~CommandOrientTo()
{
}

void CommandOrientTo::Update(const _float& dt)
{
	if (!m_bIsControlEnd)
	{
		m_fElaspedTime += dt;
		if (m_fElaspedTime >= m_fTime)
		{
			m_pTargetObj->SetRotation(m_vRotDest);
			m_bIsControlEnd = true;
			return;
		}

		vec3 vRot = m_pTargetObj->GetRotation();

		// x
		if (m_fElaspedTime < m_vEaseInTime.x)
		{
			m_vCurSpeed.x = m_vEaseInAccel.x * m_fElaspedTime;
			vRot.x += m_vRotDir.x * m_vCurSpeed.x * dt;
		}
		else if (m_fElaspedTime >= m_vEaseInTime.x && m_fElaspedTime < m_vEaseOutTime.x)
		{
			m_vCurSpeed.x = m_vMaxSpeed.x;
			vRot.x += m_vRotDir.x * m_vCurSpeed.x * dt;
		}
		else if (m_fElaspedTime >= m_vEaseOutTime.x && m_fElaspedTime < m_fTime)
		{
			m_vCurSpeed.x = m_vMaxSpeed.x - (m_vEaseOutAccel.x * (m_fElaspedTime - m_vEaseOutTime.x));
			vRot.x += m_vRotDir.x * m_vCurSpeed.x * dt;
		}

		// y
		if (m_fElaspedTime < m_vEaseInTime.y)
		{
			m_vCurSpeed.y = m_vEaseInAccel.y * m_fElaspedTime;
			vRot.y += m_vRotDir.y * m_vCurSpeed.y * dt;
		}
		else if (m_fElaspedTime >= m_vEaseInTime.y && m_fElaspedTime < m_vEaseOutTime.y)
		{
			m_vCurSpeed.y = m_vMaxSpeed.y;
			vRot.y += m_vRotDir.y * m_vCurSpeed.y * dt;
		}
		else if (m_fElaspedTime >= m_vEaseOutTime.y && m_fElaspedTime < m_fTime)
		{
			m_vCurSpeed.y = m_vMaxSpeed.y - (m_vEaseOutAccel.y * (m_fElaspedTime - m_vEaseOutTime.y));
			vRot.y += m_vRotDir.y * m_vCurSpeed.y * dt;
		} 

		// z
		if (m_fElaspedTime < m_vEaseInTime.z)
		{
			m_vCurSpeed.z = m_vEaseInAccel.z * m_fElaspedTime;
			vRot.z += m_vRotDir.z * m_vCurSpeed.z * dt;
		}
		else if (m_fElaspedTime >= m_vEaseInTime.z && m_fElaspedTime < m_vEaseOutTime.z)
		{
			m_vCurSpeed.z = m_vMaxSpeed.z;
			vRot.z += m_vRotDir.z * m_vCurSpeed.z * dt;
		}
		else if (m_fElaspedTime >= m_vEaseOutTime.z && m_fElaspedTime < m_fTime)
		{
			m_vCurSpeed.z = m_vMaxSpeed.z - (m_vEaseOutAccel.z * (m_fElaspedTime - m_vEaseOutTime.z));
			vRot.z += m_vRotDir.z * m_vCurSpeed.z * dt;
		}

		if (vRot.x > 360.f)
			vRot.x -= 360.f;
		else if (vRot.x < 0.f)
			vRot.x += 360.f;

		if (vRot.y > 360.f)
			vRot.y -= 360.f;
		else if (vRot.y < 0.f)
			vRot.y += 360.f;

		if (vRot.z > 360.f)
			vRot.z -= 360.f;
		else if (vRot.z < 0.f)
			vRot.z += 360.f;

		m_pTargetObj->SetRotation(vRot);
	}
}

void CommandOrientTo::Run()
{
	if (!m_bIsFinished)
		return;

	if (nullptr == m_pTargetObj)
		return;

	m_bIsFinished = false;
	m_bIsControlEnd = true;

	m_pTargetObj->AddCommand(m_eCommType, this);
}

void CommandOrientTo::SetControlStart()
{
	SettingInformation();

	m_bIsControlEnd = false;
}

void CommandOrientTo::SetCommandFinish()
{
	m_bIsFinished = true;
}

void CommandOrientTo::SettingInformation()
{
	vec3 vTargetRot = m_pTargetObj->GetRotation();

	vec3 diff(0.f);
	
	if (0.f < m_vRotDir.x)
		diff.x = m_vRotDest.x - vTargetRot.x;
	else
		diff.x = vTargetRot.x - m_vRotDest.x;
	if (0 > diff.x)
		diff.x += 360.f;

	if (0.f < m_vRotDir.y)
		diff.y = m_vRotDest.y - vTargetRot.y;
	else
		diff.y = vTargetRot.y - m_vRotDest.y;
	if (0 > diff.y)
		diff.y += 360.f;

	if (0.f < m_vRotDir.z)
		diff.z = m_vRotDest.z - vTargetRot.z;
	else
		diff.z = vTargetRot.z - m_vRotDest.z;
	if (0 > diff.z)
		diff.z += 360.f;
	

	if (Ease_Time == m_eEasyType)
	{
		m_vEaseInTime = vec3(m_fIn);
		m_vEaseOutTime = vec3(m_fTime - m_fOut);

		vec3 tempSpeed = diff / m_fTime;
		vec3 area = tempSpeed * m_fTime;

		area.x *= 2.f;
		area.y *= 2.f;
		area.z *= 2.f;

		m_vMaxSpeed = area / ((2.f * m_fTime) - m_fIn - m_fOut);
		m_vEaseInAccel = m_vMaxSpeed / m_fIn;
		m_vEaseOutAccel = m_vMaxSpeed / m_fOut;
	}
	else
	{
		vec3 inDiff = diff;
		inDiff.x *= m_fIn * 0.01f;
		inDiff.y *= m_fIn * 0.01f;
		inDiff.z *= m_fIn * 0.01f;

		vec3 outDiff = diff;
		outDiff.x *= m_fOut * 0.01f;
		outDiff.y *= m_fOut * 0.01f;
		outDiff.z *= m_fOut * 0.01f;

		m_vMaxSpeed = (diff + inDiff + outDiff) / m_fTime;
		m_vEaseInTime.x = 2.f / m_vMaxSpeed.x * inDiff.x;
		m_vEaseInTime.y = 2.f / m_vMaxSpeed.y * inDiff.y;
		m_vEaseInTime.z = 2.f / m_vMaxSpeed.z * inDiff.z;
		m_vEaseOutTime.x = m_fTime - (2.f / m_vMaxSpeed.x * outDiff.x);
		m_vEaseOutTime.y = m_fTime - (2.f / m_vMaxSpeed.y * outDiff.y);
		m_vEaseOutTime.z = m_fTime - (2.f / m_vMaxSpeed.z * outDiff.z);

		m_vEaseInAccel = m_vMaxSpeed / m_vEaseInTime;
		m_vEaseOutAccel.x = m_vMaxSpeed.x / (2.f / m_vMaxSpeed.x * outDiff.x);
		m_vEaseOutAccel.y = m_vMaxSpeed.y / (2.f / m_vMaxSpeed.y * outDiff.y);
		m_vEaseOutAccel.z = m_vMaxSpeed.z / (2.f / m_vMaxSpeed.z * outDiff.z);
	}

	m_vCurSpeed = vec3(0.f);
	m_fElaspedTime = 0.f;
}
