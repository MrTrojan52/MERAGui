#ifndef SPHINXRECOGNIZER_H
#define SPHINXRECOGNIZER_H
#include <pocketsphinx/pocketsphinx.h>
#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>
#include <string>
#include <QDebug>
#include <QObject>
#include "sphinxbase/jsgf.h"
using std::string;
class SphinxRecognizer: public QObject {
    Q_OBJECT
private:
    ps_decoder_t * _ps;
    cmd_ln_t * _config;
    ad_rec_t * _ad;

public:
    SphinxRecognizer(string hmm_path, string dict_path, string mdef_path, string grammar_path = "") {
        this->_config = cmd_ln_init(NULL, ps_args(), TRUE,
                         "-hmm", hmm_path.c_str(),
                         "-dict", dict_path.c_str(),
                         "-mdef", mdef_path.c_str(),
                         "-jsgf", grammar_path.c_str(),
                         NULL);
        this->_ps = ps_init(this->_config);
        if (this->_ps  == NULL) {
            cmd_ln_free_r(this->_config);
            qDebug() << "ERROR: ps_decoder init failed!";
        }

    }

    virtual void recognize_from_microphone(string from_device = "plughw:1,0");
    virtual void setConfig(cmd_ln_t * config);
    virtual ~SphinxRecognizer() {
        cmd_ln_free_r(this->_config);
        ad_close(_ad);
    }
signals:
    void recognized(string text) const;

public slots:
    void startRecognition(string from_device = "plughw:1,0");


};



#endif
