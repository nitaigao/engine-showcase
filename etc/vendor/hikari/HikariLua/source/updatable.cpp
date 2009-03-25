#include "StdAfx.h"
#include "Updatable.h"

std::list<CUpdatable *> CUpdatable::m_listUpdates;
std::list<CUpdatable *> CUpdatable::m_lstDeathrow;

CUpdatable::CUpdatable(void)
{
	m_listUpdates.push_back(this);
}

CUpdatable::~CUpdatable(void)
{
	m_listUpdates.remove(this);
}

void CUpdatable::DestroySelf()
{
	m_lstDeathrow.push_back(this);
}

void CUpdatable::ProcessUpdates(float fTime)
{
	while (!m_lstDeathrow.empty())
	{
		delete (m_lstDeathrow.front());
		m_lstDeathrow.pop_front();
	}

	std::list<CUpdatable *>::iterator it = m_listUpdates.begin();
	while(it != m_listUpdates.end())
	{
		if((*it))
		{
			(*it)->Update(fTime);
		}
		++it;
	}
}
