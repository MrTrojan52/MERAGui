#ifndef CUSTOMMATERIALDIALOG_H
#define CUSTOMMATERIALDIALOG_H
#include <QDialog>
#include <qtmaterialappbar.h>
#include <QLabel>
class CustomMaterialDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CustomMaterialDialog(QWidget *parent = nullptr);
    ~CustomMaterialDialog() = default;
    void setHeaderText(QString text);
    QtMaterialAppBar* getAppbar() {
        return m_appBar;
    }
    void setAppBar(QtMaterialAppBar* appbar);
private:
    QtMaterialAppBar* m_appBar = nullptr;
    QLabel* headerText = nullptr;
};

#endif // CUSTOMMATERIALDIALOG_H
