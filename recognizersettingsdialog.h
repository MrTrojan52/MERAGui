#ifndef RECOGNIZERSETTINGSDIALOG_H
#define RECOGNIZERSETTINGSDIALOG_H
#include "custommaterialdialog.h"
#include "qtmaterialappbar.h"
#include <QTextToSpeech>

namespace Ui {
class RecognizerSettingsDialog;
}

class RecognizerSettingsDialog : public CustomMaterialDialog
{
    Q_OBJECT

public:
    RecognizerSettingsDialog(QWidget *parent = nullptr, QString settings_file = "settings.ini");
    ~RecognizerSettingsDialog();
private slots:
    void fieldEdited(QString text);

    void on_tbModelSelect_clicked();

    void on_tbMdefSelect_clicked();

    void on_tbDictSelect_clicked();

    void on_tbGrammSelect_clicked();

    void saveBtnClicked();

    void on_cmbAudioDevice_currentIndexChanged(int index);

    void on_cmbEngine_currentIndexChanged(int index);
signals:
    void TTSSettingsChanged();
private:
    Ui::RecognizerSettingsDialog *ui;
    QTextToSpeech* tts = nullptr;
    void fillFieldsFromIniFile();
    bool validateAllFields();
    QString _sfilename = "recognize_settings.ini";
    bool _fieldsStateChanged = false;    
};

#endif // RECOGNIZERSETTINGSDIALOG_H
