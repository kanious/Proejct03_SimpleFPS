#ifndef _COMMANDORIENTTO_H_
#define _COMMANDORIENTTO_H_

#include "Command.h"
#include <queue>
#include <glm/vec3.hpp>

class Object;

class CommandOrientTo : public Command
{
private:
	Object*					m_pTargetObj;
	EaseType				m_eEasyType;
	glm::vec3				m_vRotDest;
	glm::vec3				m_vRotDir;

	glm::vec3				m_vMaxSpeed;
	glm::vec3				m_vCurSpeed;
	_float					m_fIn;
	_float					m_fOut;

	_float					m_fTime;
	_float					m_fElaspedTime;

	glm::vec3				m_vEaseInTime;
	glm::vec3				m_vEaseOutTime;
	glm::vec3				m_vEaseInAccel;
	glm::vec3				m_vEaseOutAccel;

private:
	explicit CommandOrientTo();
public:
	explicit CommandOrientTo(EaseType type, Object* target, glm::vec3 rotDest
		, glm::vec3 rotDir, _float fTime, _float in, _float out);
	virtual ~CommandOrientTo();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();

private:
	void SettingInformation();
};

#endif //_COMMANDORIENTTO_H_