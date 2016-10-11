#ifndef MPREXPOSUREDIALOG_H
#define MPREXPOSUREDIALOG_H

#include <QFile>
#include <QList>
#include <QDebug>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <QString>
#include <QDialog>
#include <QSpinBox>
#include <QSettings>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QSerialPortInfo>
#include <QDialogButtonBox>

#define EXP_RATE_LABEL "Exposure Frequency"
#define EXP_TIME_LABEL "Exposure Length"
#define DUTY_CYCLE_LABEL "Trigger Duty Cycle"

#define STRING_NO_CONNECTION "Device not connected"
#define STRING_IS_CONNECTION "Device found!"

#define EXP_RATE_MIN 1      // milliseconds, .001 s
#define EXP_RATE_MAX 1000   // milliseconds, 1 s
#define EXP_RATE_DEF 1

#define EXP_TIME_MIN 1      // ms
#define EXP_TIME_MAX 100    // ms
#define EXP_TIME_DEF 16     // ms

#define DUTY_CYCLE_MIN 1
#define DUTY_CYCLE_MAX 100
#define DUTY_CYCLE_DEF 10

class MPRExposureWidget : public QWidget
{
    Q_OBJECT
public:
    MPRExposureWidget(QWidget *parent = 0);
    ~MPRExposureWidget();

public slots:
    void onConnectTry();

private:
    QGroupBox *controlsGroupBox;
    QComboBox *serialComboBox;
    QSlider *expRateSlider, *dutyCycleSlider, *expTimeSlider;
    QSpinBox *expRateSpinBox, *dutyCycleSpinBox, *expTimeSpinBox;
    QLabel *connectionLabel;
    QPushButton *connectButton;

    bool isConnected;

private slots:
    void onConnectOK();
    void onConnectFail();
};


class MPRExposureDialog : public QDialog
{
    Q_OBJECT
public:
    MPRExposureDialog(QDialog *parent = 0) : QDialog(parent){
        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(10,10,10,10);

        exposureWidget = new MPRExposureWidget();
        this->layout()->addWidget(exposureWidget);
        ((QVBoxLayout*)(this->layout()))->addSpacing(20);
    }

protected:
    void accept() { QDialog::accept(); }
    void reject() { QDialog::reject(); }

public slots:

private:
    MPRExposureWidget *exposureWidget;

signals:
};

#endif // MPREXPOSUREDIALOG_H
