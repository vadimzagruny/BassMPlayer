#include "musicplayer.h"
#include <QTime>

CMusicPlayer::CMusicPlayer(QObject *parent) : QObject(parent)
{
    // init BASS library
    if (!BASS_Init(-1, 44100, 0, NULL, NULL)){
        qDebug() << "Cannot initialize device";
    }

    //setup SFX params

    // parametric equalizer params
    m_FX_ParamQ[e125HZ].params.fGain      = 0;
    m_FX_ParamQ[e125HZ].params.fBandwidth = 18;
    m_FX_ParamQ[e125HZ].params.fCenter    = 125;// 125 hz
    m_FX_ParamQ[e250HZ].params.fGain      = 0;
    m_FX_ParamQ[e250HZ].params.fBandwidth = 18;
    m_FX_ParamQ[e250HZ].params.fCenter    = 250;// 250 hz
    m_FX_ParamQ[e1KHZ ].params.fGain      = 0;
    m_FX_ParamQ[e1KHZ ].params.fBandwidth = 18;
    m_FX_ParamQ[e1KHZ ].params.fCenter    = 1000;// 1 khz
    m_FX_ParamQ[e4KHZ ].params.fGain      = 0;
    m_FX_ParamQ[e4KHZ ].params.fBandwidth = 18;
    m_FX_ParamQ[e4KHZ ].params.fCenter    = 4000;// 4khz
    m_FX_ParamQ[e8KHZ ].params.fGain      = 0;
    m_FX_ParamQ[e8KHZ ].params.fBandwidth = 18;
    m_FX_ParamQ[e8KHZ ].params.fCenter    = 8000;// 8khz

    // reverb params
    m_FX_Reverb.params.fInGain          = 0.f;
    m_FX_Reverb.params.fReverbMix       = -96.f;
    m_FX_Reverb.params.fReverbTime      = 1000.f;
    m_FX_Reverb.params.fHighFreqRTRatio = 0.001f;

    // init tick timer
    m_TickTimer = new QTimer(this);

    // notify rec offset every second
    connect(m_TickTimer,&QTimer::timeout,[this](){
        emit positionChanged(getCurPosition());
    });

    // check the end of a record
    connect(this,&CMusicPlayer::positionChanged,this,&CMusicPlayer::updateList);

    m_LoopMode      = eLIST_LOOP;
    m_iCurrentRec   = -1;
    hStream         = 0;
    m_bSFX_On       = false;
}

CMusicPlayer::~CMusicPlayer()
{
    // close output
    BASS_Free();

    freeStream(hStream);

    m_RecList.clear();
}

void CMusicPlayer::pushRecord(const QString filePath)
{
    // push file path to playlist
    m_RecList << filePath;

    //we back to start of the list after every addition
    m_iCurrentRec = 0;
}

void CMusicPlayer::SetCurRecord(int recIdx)
{
    // check index, if it is too larg we set first rec
    if(recIdx >= 0 && recIdx < m_RecList.size())
       m_iCurrentRec = recIdx;
    else
        m_iCurrentRec = 0;
}

int CMusicPlayer::getCurPosition()
{
    // get current position of the record in bytes
    int pos = BASS_ChannelGetPosition(hStream,BASS_POS_BYTE);
    // recompute bytes to m-seconds
    return (int)(BASS_ChannelBytes2Seconds(hStream,pos) * 1000);
}

int CMusicPlayer::getCurLength()
{
    // get current duration of the record in bytes
    int len = BASS_ChannelGetLength(hStream,BASS_POS_BYTE);
    // recompute bytes to m-seconds
    return (int)(BASS_ChannelBytes2Seconds(hStream,len) * 1000);
}

void CMusicPlayer::setFXEnabled(bool enable)
{
    if(!enable && m_bSFX_On){       // remove sfx
        m_bSFX_On = false;
        removeSoundEffects();
    }
    else if(!m_bSFX_On && enable){  // set sfx
        m_bSFX_On = true;
        setSoundEffects();
    }
}

void CMusicPlayer::removeSoundEffects()
{
    if(!hStream) return; // check stream

    // remove equalizer params
    for (int i=0; i<ePARAMS_NUM;i++){
        if(!BASS_ChannelRemoveFX(hStream,m_FX_ParamQ[i].hFX)){
            qDebug() << "Can't Remove FX ID: " << i;
        }
    }

    // remove reverb
    if(!BASS_ChannelRemoveFX(hStream,m_FX_Reverb.hFX)){
        qDebug() << "Can't Remove reverb FX";
    }
}

void CMusicPlayer::setSoundEffects()
{
    if(!hStream) return; // check stream

    // get parametric equalizer handlers
    m_FX_ParamQ[e125HZ].hFX  = BASS_ChannelSetFX(hStream,BASS_FX_DX8_PARAMEQ,0);
    m_FX_ParamQ[e250HZ].hFX  = BASS_ChannelSetFX(hStream,BASS_FX_DX8_PARAMEQ,0);
    m_FX_ParamQ[e1KHZ ].hFX  = BASS_ChannelSetFX(hStream,BASS_FX_DX8_PARAMEQ,0);
    m_FX_ParamQ[e4KHZ ].hFX  = BASS_ChannelSetFX(hStream,BASS_FX_DX8_PARAMEQ,0);
    m_FX_ParamQ[e8KHZ ].hFX  = BASS_ChannelSetFX(hStream,BASS_FX_DX8_PARAMEQ,0);

    // update equalizer params
    for (int i=0; i<ePARAMS_NUM;i++){
        updateParamQ_SFX((SFX_PARAMQ)i);
    }

    // get reverb handler
    m_FX_Reverb.hFX = BASS_ChannelSetFX(hStream,BASS_FX_DX8_REVERB,0);

    // update reverb
    updateReverb_SFX();
}

void CMusicPlayer::createStreamFromFile(const QString filePath)
{
    // create stream from file
    if (hStream = BASS_StreamCreateFile(false,filePath.toStdWString().c_str(),0,0,BASS_SAMPLE_FX)) {
        // init sound effects if enabled
        if(m_bSFX_On)
            setSoundEffects();
        // generate signal - duration changed
        emit changeLength(getCurLength());
    }
    else {
        qDebug() << "Can't open stream";
    }
}

void CMusicPlayer::freeStream(HSTREAM handle)
{
    if(!handle)return ;

    // free the stream
    BASS_StreamFree(handle);

    hStream = 0;
}


void CMusicPlayer::updateParamQ_SFX(SFX_PARAMQ param)
{
    BASS_FXSetParameters(m_FX_ParamQ[param].hFX, &m_FX_ParamQ[param].params);
}

void CMusicPlayer::updateReverb_SFX()
{
    BASS_FXSetParameters(m_FX_Reverb.hFX,&m_FX_Reverb.params);
}

// slots
void CMusicPlayer::setVolume(unsigned int _vol)
{
    //check _vol, it must be in range from 0 to 100
    unsigned int vol = _vol <= 100 ? _vol : 100;

    //Bass global stream volume in range(0-10000)
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM,vol*100);
}

void CMusicPlayer::play()
{
    if(m_RecList.empty())return;

    // is curent index available ?
    if(m_iCurrentRec != -1 && m_iCurrentRec!= m_RecList.size()){
        // if current stream is not created
        if(!hStream){
            createStreamFromFile(m_RecList[m_iCurrentRec]);
        }
        // start playing stream
        BASS_ChannelPlay(hStream,true);

        // init timer to notify every second
        m_TickTimer->start(1000);
    }
}
void CMusicPlayer::stop()
{
    // stop stream playing
    BASS_ChannelStop(hStream);
    // stop timer
    m_TickTimer->stop();
}

void CMusicPlayer::pause()
{
    // pause stream playing
    BASS_ChannelPause(hStream);
    m_TickTimer->stop();
}

void CMusicPlayer::resume()
{
    // resume stream playing
    BASS_ChannelPlay(hStream,false);
    // init timer to notify every second
    m_TickTimer->start(1000);
}

void CMusicPlayer::updateList(int var)
{
    // if curren position is equal to record length
    // go to next record
    int len = getCurLength();
    if(len == var){
        m_TickTimer->stop();
        next();
    }
}

void CMusicPlayer::changePosition(int pos)
{
    this->pause();

    // change record position
    BASS_ChannelSetPosition(hStream,
                            BASS_ChannelSeconds2Bytes(hStream, pos/1000.0),
                            BASS_POS_BYTE);

    this->resume();
}

void CMusicPlayer::setCurrentIndex(unsigned int idx)
{
    // stop playing
    this->stop();
    m_TickTimer->stop();

    // if index available
    // change current index of the playlist
    if(idx < (unsigned int)m_RecList.size())
    {
        // free previus stream
        freeStream(hStream);
        m_iCurrentRec = idx;

        // generate signal - index changed
        emit currentIndexChanged(m_iCurrentRec);
    }

}

void CMusicPlayer::prev()
{
    this->stop();

    switch (m_LoopMode)
    {
        case eTRACK_SINGLE:
        case eLIST_SINGLE:
        {
            if((m_iCurrentRec - 1) != -1){
                --m_iCurrentRec;
            }
        }
        break;
        case eTRACK_LOOP:
        {
            m_iCurrentRec;
        }
         break;
        case eLIST_LOOP:
        {
            if((m_iCurrentRec - 1) > -1){
                --m_iCurrentRec;
            }
            else if((m_iCurrentRec - 1) == -1){
                m_iCurrentRec = m_RecList.size() - 1;
            }
        }
        break;
        case eLIST_RAND:
        {
           qsrand(QTime::currentTime().msec());
           int  rand = abs(qrand()%(m_RecList.size()));
           m_iCurrentRec = rand;
        }
        break;
        default:
            return;
    }

    freeStream(hStream);
    // generate signal - index changed
    emit currentIndexChanged(m_iCurrentRec);
    this->play();
}

void CMusicPlayer::next()
{
    this->stop();

    switch (m_LoopMode)
    {
        case eTRACK_SINGLE:
        case eLIST_SINGLE:
        {
            if((m_iCurrentRec + 1) < m_RecList.size()){
                ++m_iCurrentRec;
            }
        }
        break;
        case eTRACK_LOOP:
        {
            m_iCurrentRec;
        }
         break;
        case eLIST_LOOP:
        {
            if((m_iCurrentRec + 1) < m_RecList.size()){
                ++m_iCurrentRec;
            }
            else if(m_iCurrentRec + 1 == m_RecList.size()){
                m_iCurrentRec = 0;
            }
        }
        break;
        case eLIST_RAND:
        {
            qsrand(QTime::currentTime().msec());
            int  rand = abs(qrand()%(m_RecList.size()));
            m_iCurrentRec = rand;

        }
        break;
        default:
            return;
    }

    freeStream(hStream);
    // generate signal - index changed
    emit currentIndexChanged(m_iCurrentRec);
    this->play();
}


// sfx

void CMusicPlayer::setFXParamQValue(SFX_PARAMQ param,int val)
{
    m_FX_ParamQ[param].params.fGain = val - 10.0;

    updateParamQ_SFX(param);
}
void CMusicPlayer::setFXRevebValue(int val)
{
    m_FX_Reverb.params.fReverbMix = (val>0 ? log(val/20.0) * 20 : -96);

    updateReverb_SFX();
}
