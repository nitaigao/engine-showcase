#pragma once

#include <list>

class CUpdatable
{
public:
	static void ProcessUpdates(float fTime);

	CUpdatable(void);
	virtual ~CUpdatable(void);

	virtual void Update(float fTime)=0;
	void	DestroySelf();

protected:
	static std::list<CUpdatable *> m_listUpdates;
	static std::list<CUpdatable *> m_lstDeathrow;
};
