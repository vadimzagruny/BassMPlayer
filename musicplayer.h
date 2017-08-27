#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QTimer>

#include "bass.h"
#include <QDebug>


class CMusicPlayer : public QObject
{
    Q_OBJECT

// private tipes
    struct SFX_PARAMQ_DATA
    {
       HFX hFX;
       BASS_DX8_PARAMEQ params;
    };
    struct SFX_REVERB_DATA
    {
        HFX hFX;
        BASS_DX8_REVERB params;
    };

public:

// public tipes
    enum SFX_PARAMQ{
        e125HZ = 0,
        e250HZ,
        e1KHZ,
        e4KHZ,
        e8KHZ,
        ePARAMS_NUM
    };

    enum LOOP_MODE{
        eTRACK_SINGLE,
        eTRACK_LOOP,
        eLIST_SINGLE,
        eLIST_LOOP,
        eLIST_RAND
    };

public:
    explicit CMusicPlayer(QObject *parent = nullptr);
    ~CMusicPlayer();

    void pushRecord(const QString filePath);
    void SetCurRecord(int recIdx);

    void SetLoopMode(LOOP_MODE mode){m_LoopMode = mode;}

    // sfx
    void setFXParamQValue(SFX_PARAMQ param,int val);
    void setFXRevebValue(int val);
    void setFXEnabled(bool enable);

signals:
    void currentIndexChanged(int);
    void changeLength(int len);
    void positionChanged(int pos);

public slots:

    // gen
    void setVolume(unsigned int vol);
    void setCurrentIndex(unsigned int idx);
    void changePosition(int pos);
    void play();
    void stop();
    void pause();
    void resume();
    void prev();
    void next();



private slots:
    void updateList(int var);


private:

    void createStreamFromFile(const QString filePath);
    void freeStream(HSTREAM handle);

    int getCurLength();
    int getCurPosition();

    void setSoundEffects();
    void removeSoundEffects();
    void updateParamQ_SFX(SFX_PARAMQ param);
    void updateReverb_SFX();

private:

    QVector <QString>m_RecList;

    QTimer *m_TickTimer;

    LOOP_MODE m_LoopMode;

    bool m_bSFX_On;
    int m_iCurrentRec;

    HSTREAM hStream;
    // 3 eq bands
    SFX_PARAMQ_DATA m_FX_ParamQ[ePARAMS_NUM];
    // reverb
    SFX_REVERB_DATA m_FX_Reverb;

};

#endif // MUSICPLAYER_H
