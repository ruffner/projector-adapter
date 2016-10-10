#ifndef MPREXPOSUREDIALOG_H
#define MPREXPOSUREDIALOG_H

#include <QFile>
#include <QList>
#include <QDebug>
#include <QLabel>
#include <QSlider>
#include <QWidget>
#include <QString>
#include <QThread>
#include <QDialog>
#include <QSpinBox>
#include <QSettings>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QDialogButtonBox>

class MPRExposureWidget : public QWidget
{
    Q_OBJECT
public:
    MPRExposureWidget(QWidget *parent = 0);
    ~MPRExposureWidget();

private:
    QSpinBox *serialSpingBox,*expRateSpinBox, *dutyCycleSpinBox, *expTimeSpinBox;
};


class MPRExposureDialog : public QDialog
{
    Q_OBJECT
public:
    MPRExposureDialog(QDialog *parent = 0) : QDialog(parent){
        this->setLayout(new QVboxLayout());
        this->layout()->setContentsMargins(10,10,10,10);
        this->setMaximunWidth(500);

        exposureWidget = new MPRExposureWidget();
        this->layout()->addWidget(exposureWidget);
        ((QVBoxLauout*)(this->layout()))->addSpacing(20);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
        this->layout()->addWidget(buttonBox);
        connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
        connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
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
