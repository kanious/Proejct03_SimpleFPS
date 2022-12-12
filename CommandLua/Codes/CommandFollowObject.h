#ifndef _COMMANDFOLLOWOBJECT_H_
#define _COMMANDFOLLOWOBJECT_H_

#include "Command.h"
#include <queue>
#include <glm/vec3.hpp>

class Object;

class CommandFollowObject : public Command
{
private:
	Object*					m_pTargetObj;
	Object*					m_pFollowObj;

	_float					m_fMaxSpeed;
	_float					m_fTime;
	_float					m_fDecelerateDist;
	_float					m_fStopDist;

	_float					m_fElaspedTime;
	_float					m_fCurSpeed;
	_float					m_fAccel;


private:
	explicit CommandFollowObject();
public:
	explicit CommandFollowObject(Object* target, Object* follow, _float maxSpeed, _float fFollowTime, _float decelerateDist, _float stopDist);
	virtual ~CommandFollowObject();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();
};

#endif //_COMMANDFOLLOWOBJECT_H_