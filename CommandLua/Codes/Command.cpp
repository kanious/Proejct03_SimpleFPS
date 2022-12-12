#include "Command.h"
#include "Object.h"

Command::Command()
	: m_eCommType(Command_End), m_bIsFinished(true), m_bIsControlEnd(true)
{
}

Command::~Command()
{
}

void Command::AddCommand(Command* pCommand)
{
}
