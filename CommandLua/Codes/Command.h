#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "EngineDefines.h"
#include "Enums.h"

class Object;

class Command
{
protected:
	CommandType					m_eCommType;
	std::string					m_strName;
	_bool						m_bIsFinished;
	_bool						m_bIsControlEnd;

public:
	explicit Command();
	virtual ~Command();

public:
	_bool isFinished()				{ return m_bIsFinished; }
	_bool isControlEnd()			{ return m_bIsControlEnd; }
	CommandType GetCommandType()	{ return m_eCommType; }
public:
	virtual void Update(const _float& dt) = 0;
	virtual void Run() = 0;
	virtual void SetControlStart() = 0;
	virtual void SetCommandFinish() = 0;
public:
	virtual void AddCommand(Command* pCommand);

};

#endif //_COMMAND_H_