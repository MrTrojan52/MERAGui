#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include "Device/include/switchdevice.h"
#include <lib/qtmaterialtheme.h>
#include <QLabel>
#include "Factories/include/devicefactory.h"
#include <QScrollArea>
#include <qtmaterialscrollbar.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setSizeGripEnabled(true);
    this->setWindowFlag(Qt::FramelessWindowHint);
    QLabel *label = new QLabel("Sphinx", this);
    label->setAttribute(Qt::WA_TranslucentBackground);
    label->setForegroundRole(QPalette::Foreground);
    label->setContentsMargins(6, 0, 0, 0);

    QPalette palette = label->palette();
    palette.setColor(label->foregroundRole(), Qt::white);
    label->setPalette(palette);

    label->setFont(QFont("Roboto", 18, QFont::Normal));

    m_recognizeBtn = new QtMaterialIconButton(QtMaterialTheme::icon("av", "mic_off"),this);
    m_recognizeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    m_recognizeBtn->setIconSize(QSize(24, 24));
    m_recognizeBtn->setColor(Qt::white);
    m_recognizeBtn->setFixedWidth(42);
    connect(m_recognizeBtn, SIGNAL(clicked()), this, SLOT(startRecognizeClicked()));

    m_settingsBtn = new QtMaterialIconButton(QtMaterialTheme::icon("action", "settings"));
    m_settingsBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    m_settingsBtn->setIconSize(QSize(24, 24));
    m_settingsBtn->setColor(Qt::white);
    m_settingsBtn->setFixedWidth(42);
    connect(m_settingsBtn, SIGNAL(clicked()), this, SLOT(recognitionSettingsClicked()));

    m_appBar = new QtMaterialAppBar(this);
    m_appBar->setFixedHeight(65);
    m_appBar->appBarLayout()->addWidget(m_recognizeBtn);
    m_appBar->appBarLayout()->addWidget(label, 1);
    m_appBar->appBarLayout()->addWidget(m_settingsBtn);
    m_appBar->appBarLayout()->setAlignment(m_settingsBtn, Qt::AlignRight);
    m_appBar->setCursor(QCursor(Qt::CursorShape::OpenHandCursor));
    m_appBar->installEventFilter(this);
    ui->verticalLayout->insertWidget(0, m_appBar);

    QHBoxLayout* toolBtnsHLayout = new QHBoxLayout(ui->verticalLayout->widget());
    QToolButton * closeBtn = new QToolButton(this);
    closeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    closeBtn->setIcon(QIcon(":/images/assets/cross.png"));
    closeBtn->resize(16, 16);
    closeBtn->setIconSize(QSize(16,16));
    closeBtn->setStyleSheet("QToolButton {\
                                border: 1px solid #ABABAB;\
                                background-color: #EE7B42;\
                            }\
                            QToolButton:hover {\
                                background-color: #FFA642;\
                            }");
    QToolButton * minimizeBtn = new QToolButton(this);
    minimizeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    minimizeBtn->setIcon(QIcon(":/images/assets/underline.png"));
    minimizeBtn->resize(16, 16);
    minimizeBtn->setIconSize(QSize(16,16));
    minimizeBtn->setStyleSheet("QToolButton {\
                                border: 1px solid #ABABAB;\
                                background-color: #00BCD4;\
                            }\
                            QToolButton:hover {\
                                background-color: #88CFDF;\
                            }");
    toolBtnsHLayout->addWidget(minimizeBtn, 0, Qt::AlignRight);
    toolBtnsHLayout->addWidget(closeBtn);
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(closeBtnClicked()));
    connect(minimizeBtn, SIGNAL(clicked()), this, SLOT(minimizeBtnClicked()));
    ui->verticalLayout->insertLayout(0, toolBtnsHLayout);

    addButton = new QToolButton(this);
    addButton->setText("+");
    addButton->setToolTip("Добавить устройство в группу");
    addButton->setIcon(QIcon(":/images/assets/plus.png"));
    addButton->setStyleSheet("QToolButton {\
                             border: 1px solid #ABABAB;\
                             background-color: white;\
                             }\
                             QToolButton:hover {\
                                 background-color: #E1E1E1;\
                             }");
    addButton->resize(20,20);
    addButton->setIconSize(QSize(20,20));
    addButton->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    ui->tabDevices->setCornerWidget(addButton, Qt::Corner::BottomRightCorner);

    ttsSett = getTTSSettings();
    tts = new QTextToSpeech(ttsSett->getEngine(), this);
    tts->setVoice(ttsSett->getVoice());
    addDlg = new AddDeviceDialog(this, _sdevicesFilename);
    cdlg = new ConnectionDialog(this, _sfilename);
    rsDlg = new RecognizerSettingsDialog(this, _sfilename);
    cdlg->setModal(true);
    cdlg->show();
    connect(cdlg, &ConnectionDialog::accepted, this, &Dialog::connectionDialogAccepted);
    connect(cdlg, &ConnectionDialog::rejected, this, &Dialog::connectionDialogRejected);
    connect(addButton, &QToolButton::clicked, this, &Dialog::addButtonClicked);
    connect(addDlg, &AddDeviceDialog::deviceListChanged, this, &Dialog::updateDevices);
    connect(rsDlg, &RecognizerSettingsDialog::TTSSettingsChanged, this, &Dialog::onTTSSettingsChanged);
    connect(tts, &QTextToSpeech::stateChanged, this, &Dialog::ttsStateChanged);
}

Dialog::~Dialog()
{
    delete ui;
    delete ttsSett;
    _recognizer->deleteLater();
    delete _mclient;
    delete ConnectionManager;
    for(size_t i = 0; i < _devices.size(); ++i)
        delete _devices[i];

    if(_recognizeThread) {
        _recognizeThread->exit();
        _recognizeThread->deleteLater();
    }

}

void Dialog::onRecognize(string command) {
    qDebug() << QString::fromStdString(command);
    for(auto x : _devices) {
        x->checkTrigger(QString::fromStdString(command));
    }
}

void Dialog::brokerDisconnected() {
    QMessageBox::warning(this, QLatin1String("Error"), QLatin1String("Client disconnect from broker."));
}

void Dialog::updateLogStateChange() {
    qDebug() << "State changed to " << _mclient->state();
    if(_mclient->state() == QMqttClient::Connected) {
        auto subscription = _mclient->subscribe(QString(ConnectionManager->getGeneralFeedUrl()));
        if (!subscription) {
            QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
            this->cdlg->show();
            return;
        }
    }
}

void Dialog::connectionDialogAccepted() {
    this->_cData = cdlg->getData();
    ConnectionManager = resolveConnectionManagerByHostName(_cData.Host);
    if(ConnectionManager) {
        initMQTTClient();
        getAllFeeds();
    } else {
        QMessageBox::warning(this, "Внимание", "Неизвестный хост!");
    }
}

void Dialog::connectionDialogRejected() {
    if(QMessageBox::warning(this, "Внимание", "Закрытие данного диалогового окна приведет к прекращению работы программы!\n Вы уверены, что хотите выйти?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
        cdlg->show();
    } else {
        this->close();
    }

}

IConnectionManager* Dialog::resolveConnectionManagerByHostName(QString host) {
    if(host.toLower() == "io.adafruit.com")
        return new AdafruitConnectionManager(_cData);
    else
        return nullptr;
}

void Dialog::updateDevices() {
    QFile jsonFile(_sdevicesFilename);
    if(jsonFile.open(QFile::ReadOnly)) {
        DeviceFactory dF;
        QJsonDocument jDoc(QJsonDocument::fromJson(jsonFile.readAll()));
        QJsonObject jObj = jDoc.object();
        if(jObj.find("devices") != jObj.end()) {
            if(jObj["devices"].isArray()) {
                m_recognizeBtn->setEnabled(false);
                if(this->_recognizer) {
                    this->m_recognizeBtnChecked = true;
                    this->m_recognizeBtn->setIcon(QtMaterialTheme::icon("av","mic_off"));
                    this->_recognizer->stopRecognition();
                }
                for(size_t i = 0; i < _devices.size(); ++i)
                    delete _devices[i];
                _devices.clear();
                QJsonArray jDevArray = jObj["devices"].toArray();
                int id = 0;
                for(int i = 0; i < jDevArray.size(); ++i) {
                    QJsonObject obj = jDevArray[i].toObject();
                    if(obj.find("group") != obj.end() && obj.find("topic") != obj.end())
                    {
                        if(availableDevicesByGroup.find(obj["group"].toString()) != availableDevicesByGroup.end())
                        {
                            vector<AvailableDeviceInfo> Adevices = availableDevicesByGroup[obj["group"].toString()];
                            bool finded = false;
                            QString topic = obj["topic"].toString();
                            for(auto x : Adevices)
                            {
                                if(x.getFeed() == topic)
                                {
                                    finded = true;
                                    break;
                                }
                            }
                            if(finded) {                                
                                ADevice* dev = dF.create(obj);
                                if(dev){
                                    dev->setMqttClient(_mclient);
                                    dev->setFeedBaseUrl(ConnectionManager->getBaseFeedUrl());
                                    dev->setLastValueFromUrl(ConnectionManager->getLastValueUrl(dev));
                                    dev->setTTS(tts);
                                    _devices.push_back(dev);
                                    connect(dev->getDeleteAction(), &QAction::triggered, this, [this, id](bool checked){
                                        Q_UNUSED(checked)
                                        this->deleteDeviceByIndex(id);
                                    });                                   
                                }

                            }
                        }
                    }
                    ++id;
                }
                generateControls();
            }
        }
    }
}

void Dialog::initMQTTClient() {
    _mclient = new QMqttClient(this);
    _mclient->setHostname(_cData.Host);
    _mclient->setPort(_cData.Port.toInt());
    _mclient->setUsername(_cData.Username);
    _mclient->setPassword(_cData.Password);
    connect(_mclient, &QMqttClient::stateChanged, this, &Dialog::updateLogStateChange);
    connect(_mclient, &QMqttClient::disconnected, this, &Dialog::brokerDisconnected);
    connect(_mclient, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
            for(auto x: _devices)
            {
                if(x->getFeedBaseUrl() + x->getFeed() == topic.name())
                    x->setValueFromBack(message);
            }
        });
    _mclient->connectToHost();
}

void Dialog::getAllFeeds() {
    availableDevicesByGroup = ConnectionManager->getDevicesList();
    for(auto& x : availableDevicesByGroup) {
        QScrollArea* scrollArea = new QScrollArea(ui->tabDevices);
        QWidget* widget = new QWidget(scrollArea);
        QVBoxLayout* vLay = new QVBoxLayout(widget);
        vLay->setAlignment(Qt::AlignTop);
        scrollArea->setWidget(widget);
        scrollArea->setWidgetResizable(true);
        scrollArea->setVerticalScrollBar(new QtMaterialScrollBar(scrollArea));
        scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->tabDevices->addTab(scrollArea, x.first);
    }
    updateDevices();

}

void Dialog::generateControls() {
    for(auto x : _devices) {
        for(int i = 0; i < ui->tabDevices->count(); ++i)
        {
            if(ui->tabDevices->tabText(i) == x->getGroup())
            {                
                QScrollArea* sc = dynamic_cast<QScrollArea*>(ui->tabDevices->widget(i));
                if(sc)
                    x->insertWidgetsIntoLayout(sc->widget()->layout());
                else
                    x->insertWidgetsIntoLayout(ui->tabDevices->widget(i)->layout());
            }
        }
    }
    m_recognizeBtn->setEnabled(true);

}

void Dialog::deleteDeviceByIndex(int index) {
    QFile jsonFile(_sdevicesFilename);
    if(jsonFile.open(QFile::ReadOnly)) {
        QJsonDocument jDoc(QJsonDocument::fromJson(jsonFile.readAll()));
        jsonFile.close();
        QJsonObject jObj = jDoc.object();
        if(jObj.find("devices") != jObj.end()) {
            if(jObj["devices"].isArray()) {
                QJsonArray jDevArray = jObj["devices"].toArray();
                if(index >= 0 && index < jDevArray.size())
                {
                    jDevArray.removeAt(index);
                    jObj["devices"] = jDevArray;
                    jDoc.setObject(jObj);
                    jsonFile.open(QFile::WriteOnly);
                    jsonFile.write(jDoc.toJson());
                    jsonFile.close();
                    updateDevices();
                }
            }
        }
    }
}

void Dialog::recognitionSettingsClicked()
{
    rsDlg->show();
}

void Dialog::startRecognizeClicked()
{
    if(this->m_recognizeBtnChecked) {
        this->m_recognizeBtnChecked = false;
        this->m_recognizeBtn->setIcon(QtMaterialTheme::icon("av","mic"));
        QSettings sett(_sfilename, QSettings::IniFormat);
        QString model = sett.value("SPHINX/MODEL").toString();
        QString mdef = sett.value("SPHINX/MDEF").toString();
        QString dict = sett.value("SPHINX/DICTIONARY").toString();
        QString gramm = sett.value("SPHINX/GRAMMAR").toString();
        QString aDev = sett.value("SPHINX/ADEVICE").toString();
        if(model.isEmpty() || mdef.isEmpty() || dict.isEmpty() || gramm.isEmpty() || aDev.isEmpty())
        {         
            this->m_recognizeBtn->setIcon(QtMaterialTheme::icon("av","mic_off"));
            QMessageBox::critical(this, "Ошибка", "Невозможно начать распознавание!\nЗаданы не все параметры распознавания.");
            if(rsDlg) {
                rsDlg->show();
                return;
            } else {
                rsDlg = new RecognizerSettingsDialog(this, _sfilename);
                rsDlg->show();
                return;
            }
        }
        if(_recognizeThread) {
            _recognizeThread->quit();
            _recognizeThread->wait();
            delete _recognizeThread;
        }

        if(_recognizer)
            _recognizer->deleteLater();

        this->_recognizer = new SphinxRecognizer(model.toStdString(), dict.toStdString(), mdef.toStdString(), gramm.toStdString());
        _recognizeThread = new QThread;
        this->_recognizer->moveToThread(_recognizeThread);
        this->m_sphinxStarted = true;
        connect(this, SIGNAL(startRecognition(string)), this->_recognizer, SLOT(startRecognition(string)));
        connect(this->_recognizer, SIGNAL(recognized(string)), this, SLOT(onRecognize(string)));
        _recognizeThread->start();
        emit this->startRecognition(aDev.toStdString());
    } else {
        this->m_sphinxStarted = false;
        this->m_recognizeBtnChecked = true;
        this->m_recognizeBtn->setIcon(QtMaterialTheme::icon("av","mic_off"));
        if(this->_recognizer)
            this->_recognizer->stopRecognition();
    }
}

void Dialog::addButtonClicked() {
    QString tabText = ui->tabDevices->tabText(ui->tabDevices->currentIndex());
    addDlg->selectDevice(this->availableDevicesByGroup[tabText], tabText);
}

void Dialog::closeBtnClicked() {
    this->close();
}

void Dialog::minimizeBtnClicked() {
    this->setWindowState(Qt::WindowState::WindowMinimized);
}

TTSSettings* Dialog::getTTSSettings() {
    QSettings sett(_sfilename, QSettings::IniFormat);
    QString engine = sett.value("TTS/ENGINE").toString();
    QString voice = sett.value("TTS/VOICE").toString();
    return new TTSSettings(engine, voice);
}

void Dialog::onTTSSettingsChanged() {
    if(ttsSett)
        delete ttsSett;
    ttsSett = getTTSSettings();
    if(tts && ttsSett) {
        disconnect(tts, &QTextToSpeech::stateChanged, this, &Dialog::ttsStateChanged);
        delete tts;
        tts = new QTextToSpeech(ttsSett->getEngine(), this);
        tts->setVoice(ttsSett->getVoice());
        connect(tts, &QTextToSpeech::stateChanged, this, &Dialog::ttsStateChanged);
        for(auto& x : _devices) {
            x->setTTS(tts);
        }
    }

}

void Dialog::ttsStateChanged(QTextToSpeech::State state) {
    if(state == QTextToSpeech::Ready) {
        if(!this->m_sphinxStarted) {
            this->m_recognizeBtnChecked = true;
            startRecognizeClicked();
        }
    } else {
        this->m_recognizeBtnChecked = false;
        startRecognizeClicked();
    }
}

bool Dialog::eventFilter(QObject* object, QEvent* event) {
    if(object == m_appBar) {        
        QtMaterialAppBar* bar = static_cast<QtMaterialAppBar*>(object);
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent* m_event = static_cast<QMouseEvent*>(event);
            if(m_event->button() == Qt::LeftButton) {
                bar->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));
                mpos = m_event->pos();
                return true;
            }
        } else if(event->type() == QEvent::MouseButtonRelease) {
            bar->setCursor(QCursor(Qt::CursorShape::OpenHandCursor));
            mpos = QPoint(-1,-1);
            return true;
        } else if(event->type() == QEvent::MouseMove) {
            QMouseEvent* m_event = static_cast<QMouseEvent*>(event);
            if(mpos.x() >= 0 && m_event->buttons() && Qt::LeftButton) {
                QPoint diff = m_event->pos() - mpos;
                QPoint newpos = this->pos() + diff;
                this->move(newpos);
            }
            return true;
        } else {
            return false;
        }
    }
    return false;
}
