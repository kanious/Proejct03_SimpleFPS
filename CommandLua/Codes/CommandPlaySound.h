#ifndef _COMMANDPLAYSOUND_H_
#define _COMMANDPLAYSOUND_H_

#include "Command.h"
#include <queue>
#include <glm/vec3.hpp>

class Object;

class CommandPlaySound : public Command
{
private:
	std::string			m_soundTag;

private:
	explicit CommandPlaySound();
public:
	explicit CommandPlaySound(std::string soundTag);
	virtual ~CommandPlaySound();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();
};

#endif //_COMMANDPLAYSOUND_H_