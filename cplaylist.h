#ifndef CPLAYLIST_H
#define CPLAYLIST_H

#include <QObject>
#include <QList>
#include "musicplayer.h"

class CPlayList : public QObject
{
    Q_OBJECT
public:
    explicit CPlayList(CMusicPlayer *pMPlayer);
    ~CPlayList();
    void pushRecord(const QString &rec);

signals:

public slots:
    void next();
    void prev();

private:
    CMusicPlayer *m_pMPlayer;
    QList<QString>m_recdList;
};

#endif // CPLAYLIST_H
