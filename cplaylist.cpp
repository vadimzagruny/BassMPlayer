#include "cplaylist.h"

CPlayList::CPlayList(CMusicPlayer *pPlayer) :
    QObject(pPlayer),
    m_pMPlayer(pPlayer)
{

}

CPlayList::~CPlayList()
{

}

void CPlayList::next()
{

}
void CPlayList::prev()
{

}


void CPlayList::pushRecord(const QString &rec)
{

}
