#ifndef _COMMANDPARALLEL_H_
#define _COMMANDPARALLEL_H_

#include "Command.h"

class CommandParallel : public Command
{
private:
	std::vector<Command*>		m_vecCommands;

private:
	explicit CommandParallel();
public:
	explicit CommandParallel(CommandType type, std::string name);
	virtual ~CommandParallel();

public:
	virtual void Update(const _float& dt);
	virtual void Run();
	virtual void SetControlStart();
	virtual void SetCommandFinish();

public:
	virtual void AddCommand(Command* pCommand);

};

#endif //_COMMANDPARALLEL_H_