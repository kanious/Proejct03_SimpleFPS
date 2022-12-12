#ifndef _COMMANDSTAY_H_
#define _COMMANDSTAY_H_

#include "Command.h"
#include <queue>
#include <glm/vec3.hpp>

class Object;

class CommandStay : public Command
{
private:
	Object*				m_pTargetObj;
	_float				m_fTime;
	_float				m_fElaspedTime;

private:
	explicit CommandStay();
public:
	explicit CommandStay(Object* target, _float fTime);
	virtual ~CommandStay();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();
};

#endif //_COMMANDSTAY_H_