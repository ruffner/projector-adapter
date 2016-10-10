#include "mprexposuredialog.h"

MPRExposureDialog::MPRExposureDialog(QDialog *parent)
{
    this->setMinimumHeight(480);

    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0,0,0,0);

    QGroupBox *groupBox = new QGroupBox(QString("SERIAL"));
    groupBox->setLayout(new QHBoxLayout());
    groupBox->layout()->setContentsMargins(6,6,6,6);
    groupBox->layout()->addWidget(maleGLWidget);

    expRateSpinBox = new QSpinBox();
    expRateSpinBox->setMinimum(0);
    expRateSpinBox->setMaximum(100);
    expRateSpinBox->setValue(100);
    QLabel *label = new QLabel(QString("Frequency:"));
    groupBox->layout()->addWidget(label);
    groupBox->layout()->addWidget(expRateSpinBox);

}

