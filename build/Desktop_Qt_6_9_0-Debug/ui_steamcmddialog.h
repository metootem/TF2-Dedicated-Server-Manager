/********************************************************************************
** Form generated from reading UI file 'steamcmddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STEAMCMDDIALOG_H
#define UI_STEAMCMDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SteamCMDDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *txtOutput;
    QHBoxLayout *horizontalLayout;
    QLabel *lblProgress;
    QProgressBar *barProgress;
    QPushButton *btnCancel;

    void setupUi(QDialog *SteamCMDDialog)
    {
        if (SteamCMDDialog->objectName().isEmpty())
            SteamCMDDialog->setObjectName("SteamCMDDialog");
        SteamCMDDialog->resize(700, 500);
        verticalLayout = new QVBoxLayout(SteamCMDDialog);
        verticalLayout->setObjectName("verticalLayout");
        txtOutput = new QTextEdit(SteamCMDDialog);
        txtOutput->setObjectName("txtOutput");
        txtOutput->setUndoRedoEnabled(false);
        txtOutput->setReadOnly(true);

        verticalLayout->addWidget(txtOutput);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        lblProgress = new QLabel(SteamCMDDialog);
        lblProgress->setObjectName("lblProgress");

        horizontalLayout->addWidget(lblProgress);

        barProgress = new QProgressBar(SteamCMDDialog);
        barProgress->setObjectName("barProgress");
        barProgress->setValue(0);

        horizontalLayout->addWidget(barProgress);

        btnCancel = new QPushButton(SteamCMDDialog);
        btnCancel->setObjectName("btnCancel");

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SteamCMDDialog);

        QMetaObject::connectSlotsByName(SteamCMDDialog);
    } // setupUi

    void retranslateUi(QDialog *SteamCMDDialog)
    {
        SteamCMDDialog->setWindowTitle(QCoreApplication::translate("SteamCMDDialog", "SteamCMD", nullptr));
        txtOutput->setHtml(QCoreApplication::translate("SteamCMDDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Running SteamCMD...</p></body></html>", nullptr));
        lblProgress->setText(QCoreApplication::translate("SteamCMDDialog", "Progress:", nullptr));
        btnCancel->setText(QCoreApplication::translate("SteamCMDDialog", "Cancel Download", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SteamCMDDialog: public Ui_SteamCMDDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STEAMCMDDIALOG_H
