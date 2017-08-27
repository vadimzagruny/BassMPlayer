#ifndef MPLAYERWIDGET_H
#define MPLAYERWIDGET_H

#include <QWidget>
#include "musicplayer.h"
#include "equalizerdialog.h"


namespace Ui {
class MPlayerWidget;
}

class CEqualizerDialog;

class MPlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MPlayerWidget(QWidget *parent = 0);
    ~MPlayerWidget();

private slots:
    void on_addButton_clicked();

    void on_pauseButton_clicked(bool checked);

    void on_loopButton_clicked(bool checked);

    void on_randButton_clicked(bool checked);

private:
    Ui::MPlayerWidget   *ui;
    CMusicPlayer        *m_pMusicPlayer;
    CEqualizerDialog    *m_pEqualizer;

};

#endif // MPLAYERWIDGET_H
