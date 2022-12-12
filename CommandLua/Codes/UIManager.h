#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

#include "Base.h"

class LuaBrain;
class UIManager : public Engine::CBase
{
	SINGLETON(UIManager)
private:
	LuaBrain*			m_pBrain;
	char				m_chText[128];

private:
	explicit UIManager();
	~UIManager();
public:
	void Destroy();
	void RenderUI();
	RESULT Ready();
};

#endif //_UIMANAGER_H_