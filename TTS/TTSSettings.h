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
    TTSSettings() = default;
    TTSSettings(QString eng, QString voice_name) {
        m_engine = eng;
        QTextToSpeech tts(m_engine);
        for(const QVoice& voice : tts.availableVoices()) {
            if(voice.name() == voice_name) {
                m_voice = voice;
                break;
            }
        }
    }

    void setVoice(QVoice voice) {
        m_voice = voice;
    }

    void setEngine(QString engine) {
        m_engine = engine;
    }

    QVoice getVoice() {
        return m_voice;
    }

    QString getEngine() {
        return m_engine;
    }
};

#endif
