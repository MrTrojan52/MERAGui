#include "SphinxReconizer/include/SphinxRecognizer.h"


void SphinxRecognizer::recognize_from_microphone(string from_device) {
    this->_recognizeStopped = false;
    if(this->_ps != NULL) {

        int16 adbuf[2048];
        uint8 utt_started, in_speech;
        int32 k;
        char const *hyp;
        if ((this->_ad = ad_open_dev(from_device.c_str(),
                              (int) cmd_ln_float32_r(this->_config,
                                                     "-samprate"))) == NULL)
            E_FATAL("Failed to open audio device\n");
        if (ad_start_rec(this->_ad) < 0)
            E_FATAL("Failed to start recording\n");

        if (ps_start_utt(this->_ps) < 0)
            E_FATAL("Failed to start utterance\n");
        utt_started = FALSE;
        E_INFO("Ready....\n");

        for (;;) {
            if(_recognizeStopped) break;
            if ((k = ad_read(this->_ad, adbuf, 2048)) < 0)
                E_FATAL("Failed to read audio\n");
            ps_process_raw(this->_ps, adbuf, k, FALSE, FALSE);
            in_speech = ps_get_in_speech(this->_ps);
            if (in_speech && !utt_started) {
                utt_started = TRUE;
                E_INFO("Listening...\n");
            }
            if (!in_speech && utt_started) {
                /* speech -> silence transition, time to start new utterance  */
                ps_end_utt(this->_ps);
                hyp = ps_get_hyp(this->_ps, NULL );
                if (hyp != NULL) {
                    //printf("%s\n", hyp);
                    emit this->recognized(string(hyp));
                    fflush(stdout);
                }

                if (ps_start_utt(this->_ps) < 0)
                    E_FATAL("Failed to start utterance\n");
                utt_started = FALSE;
                E_INFO("Ready....\n");
            }
            struct timeval tmo;

            tmo.tv_sec = 0;
            tmo.tv_usec = 100 * 1000;

            select(0, NULL, NULL, NULL, &tmo);
        }
        ad_close(this->_ad);
    }
}


void SphinxRecognizer::setConfig(cmd_ln_t * config) {
    cmd_ln_free_r(this->_config);
    this->_config = config;
}

void SphinxRecognizer::startRecognition(string from_device) {
    recognize_from_microphone(from_device);
}

void SphinxRecognizer::stopRecognition() {
    _recognizeStopped = true;
}
