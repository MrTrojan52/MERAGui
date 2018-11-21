#ifndef CUSTOMMATERIALDIALOG_H
#define CUSTOMMATERIALDIALOG_H
#include <QDialog>
#include <qtmaterialappbar.h>
#include <QLabel>
#include <QToolButton>
#include <QMouseEvent>

class CustomMaterialDialog : public QDialog
{
    Q_OBJECT
public:
    bool eventFilter(QObject* object, QEvent *event);
    explicit CustomMaterialDialog(QWidget *parent = nullptr);
    ~CustomMaterialDialog() {
        if(closeBtn)
            delete closeBtn;
    }
    void setHeaderText(QString text);

    QtMaterialAppBar* getAppbar() {
        return m_appBar;
    }

    void setAppBar(QtMaterialAppBar* appbar);
private slots:
    void closeBtnClicked();
private:
    QtMaterialAppBar* m_appBar = nullptr;
    QLabel* headerText = nullptr;
    QPoint mpos;
protected:
    QToolButton* closeBtn = nullptr;
};

#endif // CUSTOMMATERIALDIALOG_H
