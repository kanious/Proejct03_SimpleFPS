#ifndef _COMMANDSERIAL_H_
#define _COMMANDSERIAL_H_

#include "Command.h"

class CommandSerial : public Command
{
private:
	std::vector<Command*>		m_vecCommands;
	Command*					m_pCurComm;
	_uint						m_iRepeat;
	_uint						m_iCurRepeat;
	_uint						m_iCurIndex;

private:
	explicit CommandSerial();
public:
	explicit CommandSerial(CommandType type, std::string name, _uint repeat);
	virtual ~CommandSerial();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();

public:
	virtual void AddCommand(Command* pCommand);
	
};

#endif //_COMMANDSERIAL_H_