#include "custommaterialdialog.h"
#include <lib/qtmaterialtheme.h>
CustomMaterialDialog::CustomMaterialDialog(QWidget* parent) :
    QDialog (parent)
{
    this->setWindowFlag(Qt::FramelessWindowHint);

    headerText = new QLabel("Inbox", this);
    headerText->setAttribute(Qt::WA_TranslucentBackground);
    headerText->setForegroundRole(QPalette::Foreground);
    headerText->setContentsMargins(6, 0, 0, 0);

    QPalette palette = headerText->palette();
    palette.setColor(headerText->foregroundRole(), Qt::white);
    headerText->setPalette(palette);

    headerText->setFont(QFont("Roboto", 18, QFont::Normal));

    m_appBar = new QtMaterialAppBar(this);
    m_appBar->appBarLayout()->addWidget(headerText);
    m_appBar->appBarLayout()->setAlignment(headerText, Qt::AlignCenter);
    m_appBar->setFixedHeight(65);

    closeBtn = new QToolButton;
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
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(closeBtnClicked()));

}


void CustomMaterialDialog::setHeaderText(QString text) {
    if(this->headerText)
        this->headerText->setText(text);
}

void CustomMaterialDialog::setAppBar(QtMaterialAppBar* appbar) {
    if(this->m_appBar)
        delete m_appBar;
    if(this->headerText)
        delete headerText;
    m_appBar = appbar;
}

void CustomMaterialDialog::closeBtnClicked() {
    emit reject();
}
