#ifndef TTSSETTINGS_H
#define TTSSETTINGS_H
#include <QString>
#include <QTextToSpeech>
#include <QVector>
class TTSSettings {
private:
    QString m_engine;
    QVoice m_voice;
public:
    TTSSettings(QString eng, QString voice_name);

    void setVoice(QVoice voice);

    void setEngine(QString engine);

    QVoice getVoice();

    QString getEngine();
};

#endif
