#ifndef _COMMANDMOVETO_H_
#define _COMMANDMOVETO_H_

#include "Command.h"
#include <queue>
#include <glm/vec3.hpp>

class Object;

class CommandMoveTo : public Command
{
private:
	Object*					m_pTargetObj;
	EaseType				m_eEasyType;
	glm::vec3				m_vDest;
	glm::vec3				m_vDir;

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
	explicit CommandMoveTo();
public:
	explicit CommandMoveTo(EaseType type, Object* target, glm::vec3 destPos, _float fTime, _float in, _float out);
	virtual ~CommandMoveTo();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();

private:
	void SettingInformation();
};

#endif //_COMMANDMOVETO_H_