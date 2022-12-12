#ifndef _COMMANDROTSUBPART_H_
#define _COMMANDROTSUBPART_H_

#include "Command.h"
#include <queue>
#include <glm/vec3.hpp>

class Object;

class CommandRotSubPart : public Command
{
private:
	Object*				m_pTargetObj;
	_float				m_fTime;
	_float				m_fElaspedTime;

private:
	explicit CommandRotSubPart();
public:
	explicit CommandRotSubPart(Object* target, _float fTime);
	virtual ~CommandRotSubPart();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();
};

#endif //_COMMANDROTSUBPART_H_