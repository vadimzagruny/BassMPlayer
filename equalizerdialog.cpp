#include "equalizerdialog.h"
#include "ui_equalizerdialog.h"

CEqualizerDialog::CEqualizerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CEqualizerDialog)
{
    ui->setupUi(this);

    // set equalaizer sliders range and start value
    ui->s125hzSlider->setRange(0,20);
    ui->s125hzSlider->setValue(10);

    ui->s250hzSlider->setRange(0,20);
    ui->s250hzSlider->setValue(10);

    ui->s1khzSlider->setRange(0,20);
    ui->s1khzSlider->setValue(10);

    ui->s4khzSlider->setRange(0,20);
    ui->s4khzSlider->setValue(10);

    ui->s8khzSlider->setRange(0,20);
    ui->s8khzSlider->setValue(10);

    //set reverb slider range and start value
    ui->reverbSlider->setRange(0,20);
    ui->reverbSlider->setValue(0);
}

CEqualizerDialog::~CEqualizerDialog()
{
    delete ui;
}

void CEqualizerDialog::on_s125hzSlider_sliderMoved(int position)
{
    emit slider125hzChanged(position);
}

void CEqualizerDialog::on_s1khzSlider_sliderMoved(int position)
{
    emit slider1khzChanged(position);
}

void CEqualizerDialog::on_s8khzSlider_sliderMoved(int position)
{
    emit slider8khzChanged(position);
}

void CEqualizerDialog::on_reverbSlider_sliderMoved(int position)
{
    emit sliderReverbChanged(position);
}

void CEqualizerDialog::on_s250hzSlider_sliderMoved(int position)
{
    emit slider250hzChanged(position);
}

void CEqualizerDialog::on_s4khzSlider_sliderMoved(int position)
{
    emit slider4khzChanged(position);
}

void CEqualizerDialog::on_onoffButton_clicked(bool checked)
{
    emit onoffButtonClicked(checked);
}

void CEqualizerDialog::on_resetButton_clicked()
{
    //reset Equalizer sliders
    ui->s125hzSlider->setValue(10);
    ui->s250hzSlider->setValue(10);
    ui->s1khzSlider->setValue(10);
    ui->s4khzSlider->setValue(10);
    ui->s8khzSlider->setValue(10);

    // send signals
    emit slider125hzChanged(10);
    emit slider250hzChanged(10);
    emit slider1khzChanged(10);
    emit slider4khzChanged(10);
    emit slider8khzChanged(10);

    //reset reverb slider
    ui->reverbSlider->setValue(0);

    //send signal
    emit sliderReverbChanged(0);

}
