#include "custommaterialdialog.h"
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
