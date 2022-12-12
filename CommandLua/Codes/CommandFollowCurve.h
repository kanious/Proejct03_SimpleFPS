#ifndef _COMMANDFOLLOWCURVE_H_
#define _COMMANDFOLLOWCURVE_H_

#include "Command.h"
#include <queue>
#include <glm/vec3.hpp>

class Object;

class CommandFollowCurve : public Command
{
private:
	Object*					m_pTargetObj;
	EaseType				m_eEasyType;
	glm::vec3				m_vDest;
	glm::vec3				m_vViaPoint;
	glm::vec3				m_vDir1;
	glm::vec3				m_vDir2;
	glm::vec3				m_vOrigin;

	_bool					m_bIsBezier;
	_bool					m_bIsFirstStep;

	_float					m_fMaxSpeed;
	_float					m_fCurSpeed;
	_float					m_fIn;
	_float					m_fOut;

	_float					m_fTime;
	_float					m_fElaspedTime;
	_float					m_fEaseInTime;
	_float					m_fEaseOutTime;

	_float					m_fEaseInAccel;
	_float					m_fEaseOutAccel;

private:
	explicit CommandFollowCurve();
public:
	explicit CommandFollowCurve(EaseType type, Object* target, glm::vec3 destPos, glm::vec3 viaPos, _float fTime, _float in, _float out, _bool isBezier);
	virtual ~CommandFollowCurve();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();

private:
	void SettingInformation();
};

#endif //_COMMANDFOLLOWCURVE_H_