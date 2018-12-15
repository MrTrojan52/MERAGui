#include "TTSSettings.h"


TTSSettings::TTSSettings(QString eng, QString voice_name) {
    m_engine = eng;
    QTextToSpeech* _tts = new QTextToSpeech(m_engine);
    for(QVoice voice : _tts->availableVoices()) {
        if(voice.name() == voice_name) {
            m_voice = voice;
            break;
        }
    }
    _tts->deleteLater();
}


void TTSSettings::setVoice(QVoice voice) {
    m_voice = voice;
}

void TTSSettings::setEngine(QString engine) {
    m_engine = engine;
}

QVoice TTSSettings::getVoice() {
    return m_voice;
}

QString TTSSettings::getEngine() {
    return m_engine;
}
