#ifndef RECOGNIZERSETTINGSDIALOG_H
#define RECOGNIZERSETTINGSDIALOG_H

#include <QDialog>
#include "qtmaterialappbar.h"
namespace Ui {
class RecognizerSettingsDialog;
}

class RecognizerSettingsDialog : public QDialog
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

private:
    void fillFieldsFromIniFile();
    bool validateAllFields();
    QString _sfilename = "recognize_settings.ini";
    bool _fieldsStateChanged = false;
    Ui::RecognizerSettingsDialog *ui;
    QtMaterialAppBar* m_appBar = nullptr;
};

#endif // RECOGNIZERSETTINGSDIALOG_H
