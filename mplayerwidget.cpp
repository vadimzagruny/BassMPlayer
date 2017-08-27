#include "mplayerwidget.h"
#include "ui_mplayerwidget.h"


#include <QDebug>
#include <QDir>
#include <QFileDialog>

MPlayerWidget::MPlayerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MPlayerWidget)
{
    ui->setupUi(this);

    m_pEqualizer = new CEqualizerDialog(this);
    //
    QStringList lables;
    lables << "Audio Track";
    lables << "Path to File";

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(lables);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_pMusicPlayer = new CMusicPlayer(this);

    connect(ui->playButton,&QPushButton::clicked,m_pMusicPlayer,&CMusicPlayer::play);
    connect(ui->stopButton,&QPushButton::clicked,m_pMusicPlayer,&CMusicPlayer::stop);
    connect(ui->prevButton,&QPushButton::clicked,m_pMusicPlayer,&CMusicPlayer::prev);
    connect(ui->nextButton,&QPushButton::clicked,m_pMusicPlayer,&CMusicPlayer::next);

    connect(ui->equalizerButton,&QPushButton::clicked,[this](){
        m_pEqualizer->show();//m_pEqualizer.exec();
    });

    connect(m_pMusicPlayer,&CMusicPlayer::changeLength,[this](int length){
        ui->trackSlider->setRange(0,length);
        ui->trackSlider->setValue(0);
        ui->durationLabel->setText(QString::number(length/1000.0/60.0,'f', 2));
    });

    connect(m_pMusicPlayer,&CMusicPlayer::positionChanged,[this](int pos){
        ui->trackSlider->setValue(pos);
        ui->curTimeLabel->setText(QString::number(pos/1000.0/60.0,'f', 2));
    });


    ui->volumeSlider->setRange(0,100);
    ui->volumeSlider->setValue(100);
    m_pMusicPlayer->setVolume(100);

    connect(ui->volumeSlider,&QSlider::sliderMoved,m_pMusicPlayer,&CMusicPlayer::setVolume);
    connect(ui->trackSlider,&QSlider::sliderMoved,m_pMusicPlayer,&CMusicPlayer::changePosition);

    connect(ui->tableWidget,&QTableWidget::doubleClicked,[this](const QModelIndex &mIdx){
        m_pMusicPlayer->setCurrentIndex(mIdx.row());
        m_pMusicPlayer->play();
    });

    connect(m_pMusicPlayer,&CMusicPlayer::currentIndexChanged,ui->tableWidget,&QTableWidget::selectRow);

    //sfx

    connect(m_pEqualizer,&CEqualizerDialog::slider125hzChanged,[this](int pos){
        m_pMusicPlayer->setFXParamQValue(CMusicPlayer::e125HZ,pos);
    });
    connect(m_pEqualizer,&CEqualizerDialog::slider250hzChanged,[this](int pos){
        m_pMusicPlayer->setFXParamQValue(CMusicPlayer::e250HZ,pos);
    });
    connect(m_pEqualizer,&CEqualizerDialog::slider1khzChanged,[this](int pos){
        m_pMusicPlayer->setFXParamQValue(CMusicPlayer::e1KHZ,pos);
    });
    connect(m_pEqualizer,&CEqualizerDialog::slider4khzChanged,[this](int pos){
        m_pMusicPlayer->setFXParamQValue(CMusicPlayer::e4KHZ,pos);
    });
    connect(m_pEqualizer,&CEqualizerDialog::slider8khzChanged,[this](int pos){
        m_pMusicPlayer->setFXParamQValue(CMusicPlayer::e8KHZ,pos);
    });
    connect(m_pEqualizer,&CEqualizerDialog::sliderReverbChanged,[this](int pos){
        m_pMusicPlayer->setFXRevebValue(pos);
    });

    connect(m_pEqualizer,&CEqualizerDialog::onoffButtonClicked,[this](bool state){
        m_pMusicPlayer->setFXEnabled(state);
    });

}

MPlayerWidget::~MPlayerWidget()
{
    delete ui;

    m_pMusicPlayer->stop();

    delete m_pMusicPlayer;
    delete m_pEqualizer;

}

void MPlayerWidget::on_addButton_clicked()
{
    QStringList strList =  QFileDialog::getOpenFileNames(this,tr("Open Files"),QString(),tr("Audio (*.mp3)"));

    int rows = ui->tableWidget->rowCount() ;

    ui->tableWidget->setRowCount(rows + strList.size());
    for(auto it :strList){
        m_pMusicPlayer->pushRecord(it);
        ui->tableWidget->setItem(rows,0,new QTableWidgetItem(QDir(it).dirName()));
        ui->tableWidget->setItem(rows,1,new QTableWidgetItem(it));
        rows++;
    }

    ui->tableWidget->resizeColumnsToContents();
}


void MPlayerWidget::on_pauseButton_clicked(bool checked)
{
    if(!checked){
        m_pMusicPlayer->resume();
    }
    else{
        m_pMusicPlayer->pause();
    }
}

void MPlayerWidget::on_loopButton_clicked(bool checked)
{
    if(checked){
        ui->randButton->setEnabled(false);
        m_pMusicPlayer->SetLoopMode(CMusicPlayer::eTRACK_LOOP);
    }
    else{
         ui->randButton->setEnabled(true);
        m_pMusicPlayer->SetLoopMode(CMusicPlayer::eLIST_LOOP);
    }
}

void MPlayerWidget::on_randButton_clicked(bool checked)
{
    if(checked){
        ui->loopButton->setEnabled(false);
        m_pMusicPlayer->SetLoopMode(CMusicPlayer::eLIST_RAND);
    }
    else{
        ui->loopButton->setEnabled(true);
        m_pMusicPlayer->SetLoopMode(CMusicPlayer::eLIST_LOOP);
    }
}
