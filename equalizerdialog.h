#ifndef EQUALIZERDIALOG_H
#define EQUALIZERDIALOG_H

#include <QDialog>

namespace Ui {
class CEqualizerDialog;
}

class CEqualizerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CEqualizerDialog(QWidget *parent = 0);
    ~CEqualizerDialog();

signals:

    void onoffButtonClicked(bool state);

    void slider125hzChanged(int val);
    void slider250hzChanged(int val);
    void slider1khzChanged(int val);
    void slider4khzChanged(int val);
    void slider8khzChanged(int val);

    void sliderReverbChanged(int val);

private slots:
    void on_s125hzSlider_sliderMoved(int position);

    void on_s1khzSlider_sliderMoved(int position);

    void on_s8khzSlider_sliderMoved(int position);

    void on_reverbSlider_sliderMoved(int position);

    void on_s250hzSlider_sliderMoved(int position);

    void on_s4khzSlider_sliderMoved(int position);

    void on_onoffButton_clicked(bool checked);

    void on_resetButton_clicked();

private:
    Ui::CEqualizerDialog *ui;
};

#endif // EQUALIZERDIALOG_H
