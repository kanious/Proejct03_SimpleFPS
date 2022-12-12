#include "CommandSerial.h"


USING(Engine)
USING(std)

CommandSerial::CommandSerial()
	: m_pCurComm(nullptr), m_iRepeat(1), m_iCurRepeat(0), m_iCurIndex(0)
{
	m_eCommType = Command_Serial;
	m_vecCommands.clear();
}

CommandSerial::CommandSerial(CommandType type, string name, _uint repeat)
	: m_pCurComm(nullptr), m_iCurRepeat(0), m_iCurIndex(0)
{
	m_eCommType = type;
	m_strName = name;
	m_iRepeat = repeat;
	m_vecCommands.clear();
}

CommandSerial::~CommandSerial()
{
	for (int i = 0; i < m_vecCommands.size(); ++i)
		delete m_vecCommands[i];
	m_vecCommands.clear();
}

void CommandSerial::Update(const _float& dt)
{
	if (m_bIsFinished)
		return;
	
	if (m_iCurIndex >= m_vecCommands.size())
	{
		++m_iCurRepeat;
		if (0 != m_iRepeat && m_iCurRepeat >= m_iRepeat)
		{
			m_bIsFinished = true;
			return;
		}
		else
		{
			m_iCurIndex = 0;
		}
	}

	if (nullptr == m_pCurComm)
	{
		m_pCurComm = m_vecCommands[m_iCurIndex];
		m_pCurComm->Run();
	}

	if (m_pCurComm->isFinished())
	{
		m_pCurComm = nullptr;
		++m_iCurIndex;
	}
	else
		m_pCurComm->Update(dt);
}

void CommandSerial::Run()
{
	if (!m_bIsFinished)
		return;

	m_bIsFinished = false;
	m_iCurIndex = 0;
	m_iCurRepeat = 0;
}

void CommandSerial::SetControlStart()
{
	m_bIsControlEnd = false;
}

void CommandSerial::SetCommandFinish()
{
	m_bIsFinished = true;
}

void CommandSerial::AddCommand(Command* pCommand)
{
	if (nullptr != pCommand)
		m_vecCommands.push_back(pCommand);
}
