#include "CommandParallel.h"
#include "CommandManager.h"


USING(Engine)
USING(std)

CommandParallel::CommandParallel()
{
	m_eCommType = Command_Serial;
	m_vecCommands.clear();
}

CommandParallel::CommandParallel(CommandType type, string name)
{
	m_eCommType = type;
	m_strName = name;
	m_vecCommands.clear();
}

CommandParallel::~CommandParallel()
{
	for (int i = 0; i < m_vecCommands.size(); ++i)
		delete m_vecCommands[i];
	m_vecCommands.clear();
}

void CommandParallel::Update(const _float& dt)
{
	if (m_bIsControlEnd)
		return;

	_bool isAllDone = true;
	for (int i = 0; i < m_vecCommands.size(); ++i)
	{
		if (!m_vecCommands[i]->isFinished())
		{
			isAllDone = false;
			m_vecCommands[i]->Update(dt);
		}
	}
	if (isAllDone)
		m_bIsControlEnd = true;
}

void CommandParallel::Run()
{
	if (!m_bIsFinished)
		return;

	m_bIsControlEnd = false;

	for (int i = 0; i < m_vecCommands.size(); ++i)
		m_vecCommands[i]->Run();

	CommandManager::GetInstance()->AddParallelCommand(this);
}

void CommandParallel::SetControlStart()
{
	m_bIsControlEnd = false;
}

void CommandParallel::SetCommandFinish()
{
	m_bIsFinished = true;
}

void CommandParallel::AddCommand(Command* pCommand)
{
	if (nullptr != pCommand)
		m_vecCommands.push_back(pCommand);
}
