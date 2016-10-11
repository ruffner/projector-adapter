#include "mprexposuredialog.h"

MPRExposureWidget::MPRExposureWidget(QWidget *parent) : QWidget(parent)
{
    this->setMinimumHeight(300);
    this->setMinimumWidth(400);

    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0,0,0,0);

    /////////////////////////////
    // DEVICE CONNECTION GROUPING
    QGroupBox *connectionGroupBox = new QGroupBox(QString("Connection"));
    connectionGroupBox->setLayout(new QHBoxLayout());
    connectionGroupBox->layout()->setContentsMargins(6,6,6,6);
    findDeviceButton = new QPushButton("Find Device", this);
    connectionLabel = new QLabel(QString(STRING_NO_CONNECTION));
    connectionGroupBox->layout()->addWidget(connectionLabel);
    connectionGroupBox->layout()->addWidget(findDeviceButton);
    this->layout()->addWidget(connectionGroupBox);

    ///////////////////////////
    // DEVICE CONTROLS GROUPING
    QGroupBox *controlsGroupBox = new QGroupBox(QString("Controls"));
    controlsGroupBox->setLayout(new QVBoxLayout());

    // EXPOSURE FREQUENCY
    QGroupBox *fGroupBox = new QGroupBox(QString(EXP_RATE_LABEL));
    fGroupBox->setLayout(new QHBoxLayout());
    expRateSlider = new QSlider(Qt::Horizontal, this);
    expRateSlider->setMinimum(EXP_RATE_MIN);
    expRateSlider->setMaximum(EXP_RATE_MAX);
    expRateSlider->setValue(EXP_RATE_DEF);
    expRateSpinBox = new QSpinBox();
    expRateSpinBox->setMinimum(EXP_RATE_MIN);
    expRateSpinBox->setMaximum(EXP_RATE_MAX);
    expRateSpinBox->setValue(EXP_RATE_DEF);
    fGroupBox->layout()->addWidget(expRateSlider);
    fGroupBox->layout()->addWidget(expRateSpinBox);
    controlsGroupBox->layout()->addWidget(fGroupBox);

    // EXPOSURE TIME
    QGroupBox *tGroupBox = new QGroupBox(QString(EXP_TIME_LABEL));
    tGroupBox->setLayout(new QHBoxLayout());
    expTimeSlider = new QSlider(Qt::Horizontal, this);
    expTimeSlider->setMinimum(EXP_TIME_MIN);
    expTimeSlider->setMaximum(EXP_TIME_MAX);
    expTimeSlider->setValue(EXP_TIME_DEF);
    expTimeSpinBox = new QSpinBox();
    expTimeSpinBox->setMinimum(EXP_TIME_MIN);
    expTimeSpinBox->setMaximum(EXP_TIME_MAX);
    expTimeSpinBox->setValue(EXP_TIME_DEF);
    tGroupBox->layout()->addWidget(expTimeSlider);
    tGroupBox->layout()->addWidget(expTimeSpinBox);
    controlsGroupBox->layout()->addWidget(tGroupBox);

    // DUTY CYCLE VARIATION
    QGroupBox *dGroupBox = new QGroupBox(QString(DUTY_CYCLE_LABEL));
    dGroupBox->setLayout(new QHBoxLayout());
    dutyCycleSlider = new QSlider(Qt::Horizontal, this);
    dutyCycleSlider->setMinimum(DUTY_CYCLE_MIN);
    dutyCycleSlider->setMaximum(DUTY_CYCLE_MAX);
    dutyCycleSlider->setValue(DUTY_CYCLE_DEF);
    dutyCycleSpinBox = new QSpinBox();
    dutyCycleSpinBox->setMinimum(DUTY_CYCLE_MIN);
    dutyCycleSpinBox->setMaximum(DUTY_CYCLE_MAX);
    dutyCycleSpinBox->setValue(DUTY_CYCLE_DEF);
    dGroupBox->layout()->addWidget(dutyCycleSlider);
    dGroupBox->layout()->addWidget(dutyCycleSpinBox);
    controlsGroupBox->layout()->addWidget(dGroupBox);

    this->layout()->addWidget(controlsGroupBox);


}

MPRExposureWidget::~MPRExposureWidget()
{

}
