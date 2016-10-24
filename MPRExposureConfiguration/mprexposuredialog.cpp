#include "mprexposuredialog.h"

MPRExposureWidget::MPRExposureWidget(QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(0,0,0,0);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);

    isConnected = false;
    serial = new QSerialPort(this);


    // DEVICE CONNECTION GROUPING
    QGroupBox *connectionGroupBox = new QGroupBox(QString("Connection"));
    connectionGroupBox->setLayout(new QHBoxLayout());
    connectionGroupBox->layout()->setContentsMargins(6,6,6,6);
    serialComboBox = new QComboBox(this);
    // ADD ALL SERIAL PORTS
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        serialComboBox->addItem(port.portName());
    }
    connectButton = new QPushButton("Connect", this);
    connectionLabel = new QLabel(QString(STRING_NO_CONNECTION));
    connectionGroupBox->layout()->addWidget(connectionLabel);
    connectionGroupBox->layout()->addWidget(serialComboBox);
    connectionGroupBox->layout()->addWidget(connectButton);
    this->layout()->addWidget(connectionGroupBox);


    // DEVICE CONTROLS GROUPING
    controlsGroupBox = new QGroupBox(QString("Controls"));
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


    // CONFIGURATION CONTROLS
    QGroupBox *bGroupBox = new QGroupBox(QString("Configuration Controls"));
    bGroupBox->setLayout(new QHBoxLayout());
    saveDeviceButton = new QPushButton(SAVE_DEVICE_LABEL, this);
    readDeviceButton = new QPushButton(READ_DEVICE_LABEL, this);
    bGroupBox->layout()->addWidget(saveDeviceButton);
    bGroupBox->layout()->addWidget(readDeviceButton);
    controlsGroupBox->layout()->addWidget(bGroupBox);

    this->layout()->addWidget(controlsGroupBox);
    controlsGroupBox->setEnabled(false);

    connect(saveDeviceButton, SIGNAL(clicked(bool)), this, SLOT(onSaveToDevice()));
    connect(readDeviceButton, SIGNAL(clicked(bool)), this, SLOT(onReadDevice()));

    connect(connectButton, SIGNAL(clicked(bool)), this, SLOT(onAttemptConnect()));
    connect(serialComboBox, SIGNAL(activated(QString)), this, SLOT(onSerialChange(QString)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    connect(expRateSpinBox, SIGNAL(valueChanged(int)), expRateSlider, SLOT(setValue(int)));
    connect(expTimeSpinBox, SIGNAL(valueChanged(int)), expTimeSlider, SLOT(setValue(int)));
    connect(dutyCycleSpinBox, SIGNAL(valueChanged(int)), dutyCycleSlider, SLOT(setValue(int)));

    connect(expRateSlider, SIGNAL(valueChanged(int)), expRateSpinBox, SLOT(setValue(int)));
    connect(expTimeSlider, SIGNAL(valueChanged(int)), expTimeSpinBox, SLOT(setValue(int)));
    connect(dutyCycleSlider, SIGNAL(valueChanged(int)), dutyCycleSpinBox, SLOT(setValue(int)));

}

MPRExposureWidget::~MPRExposureWidget()
{
    emit onDisconnect();
}

void MPRExposureWidget::onSerialChange(QString newPort)
{
    // UPDATE SERIAL PORT FROM COMBO BOX
    if( !serial->isOpen() ) {
        serial->setPortName(newPort);
    } else{
        qDebug() << "port is already open";
    }
}

void MPRExposureWidget::onAttemptConnect()
{
    if( !isConnected ){
        // SET BAUD, MAYBE OTHER SHIT
        serial->setBaudRate(QSerialPort::Baud9600);

        qDebug() << "opening serial port";

        if( serial->open(QSerialPort::ReadWrite) ){
            qDebug() << "opened serial port!";

            sendCommand(QString(COMMAND_STATUS));

        } else {
            qDebug() << "failed to open serial port";
        }
    } else {
        emit onDisconnect();
    }
}

void MPRExposureWidget::sendCommand(QString s)
{
    qDebug() << "got " << s << " in send command";

    QByteArray b(s.size(), ' ');

    for( int i=0; i<s.size(); i++ ) b[i] = s.at(i).cell();

    b.push_back('\n');
    serial->write(b);

    qDebug() << "sent '" << b << "' over serial";
}

void MPRExposureWidget::onReadyRead()
{
    data = serial->readAll();

    parseDeviceConfig();

    qDebug() << "received: " << data;
}

void MPRExposureWidget::onConnect()
{
    qDebug() << "connection successful";

    // EXPAND CONTROL PANEL
    controlsGroupBox->setEnabled(true);

    // UPDATE BUTTON
    connectButton->setText("Disconnect");

    // KEEP TRACK
    isConnected = true;

    connectionLabel->setText(QString(STRING_IS_CONNECTION));
}

void MPRExposureWidget::onDisconnect()
{
    if( serial->isOpen() ){
        qDebug() << "closing serial port";
        serial->close();
    }
    if( isConnected ) isConnected = false;

    // DISABLE CONTROLS
    controlsGroupBox->setEnabled(false);

    connectionLabel->setText(QString(STRING_NO_CONNECTION));
    connectButton->setText(QString("Connect"));
}

void MPRExposureWidget::onSaveToDevice()
{
    QString toSend = QString(COMMAND_CONFIG) + QString(" ");

    toSend += QString("%1 ").arg(expRateSpinBox->value());
    toSend += QString("%1 ").arg(expTimeSpinBox->value());
    toSend += QString("%1").arg(dutyCycleSpinBox->value());

    sendCommand(toSend);
}

void MPRExposureWidget::onReadDevice()
{
    sendCommand(QString(COMMAND_STATUS));
}

bool MPRExposureWidget::parseDeviceConfig()
{
    if( !data.toStdString().substr(0,6).compare(std::string(DEVICE_TAG)) && data.length()>11 ){
        // PREPARE UI
        if(!isConnected)
            emit onConnect();

        // HOLD INT FOR PARSING
        QString s[3];
        int vals[3];
        int sc = 0;

        for( int i=11; data[i]!='\n' && i<data.length(); i++ ){
            qDebug() << "data[i] = " << data[i];

            // BUILD UP VALUES OF INT
            if( (char)data[i] >= 48 && (char)data[i] <= 57 ){
                s[sc].append((char)data[i]);
            } else if( data[i] == ' ' || data[i] == '\r') {
                // PARSE AND SAVE
                qDebug() << "parsing " << s[sc];
                vals[sc] = s[sc].toInt();
                sc++;
            } else {
                qDebug() << "error parsing device configuration";
            }
        }

        //if( sc == 2 ){
            expRateSpinBox->setValue(vals[0]);
            expTimeSpinBox->setValue(vals[1]);
            dutyCycleSpinBox->setValue(vals[2]);

            qDebug() << "updated params from device, rate: " << vals[0] << ", time: " << vals[1] << ", duty: " << vals[2];
        //}

        return true;
    } else {
        qDebug() << "error parsing";

        emit onDisconnect();

        return false;
    }
}
