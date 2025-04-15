/********************************************************************************
** Form generated from reading UI file 'serverconsoledialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERCONSOLEDIALOG_H
#define UI_SERVERCONSOLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerConsoleDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_3;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *lblServer;
    QLabel *lblMap;
    QLabel *lblPlayers;
    QSplitter *splitter;
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QCheckBox *checkBox;
    QTextEdit *txtMainConsole;
    QHBoxLayout *horizontalLayout;
    QLabel *lblInput;
    QLineEdit *lineInput;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QTextBrowser *txtChat;
    QLabel *label;
    QTextBrowser *txtErrorConsole;

    void setupUi(QDialog *ServerConsoleDialog)
    {
        if (ServerConsoleDialog->objectName().isEmpty())
            ServerConsoleDialog->setObjectName("ServerConsoleDialog");
        ServerConsoleDialog->resize(1600, 800);
        verticalLayout_3 = new QVBoxLayout(ServerConsoleDialog);
        verticalLayout_3->setObjectName("verticalLayout_3");
        widget_3 = new QWidget(ServerConsoleDialog);
        widget_3->setObjectName("widget_3");
        widget_3->setMaximumSize(QSize(16777215, 200));
        gridLayout = new QGridLayout(widget_3);
        gridLayout->setObjectName("gridLayout");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 3, 1, 1);

        label_3 = new QLabel(widget_3);
        label_3->setObjectName("label_3");
        label_3->setMaximumSize(QSize(70, 25));

        gridLayout->addWidget(label_3, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(300, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        label_5 = new QLabel(widget_3);
        label_5->setObjectName("label_5");
        label_5->setMaximumSize(QSize(70, 25));

        gridLayout->addWidget(label_5, 2, 1, 1, 1);

        label_4 = new QLabel(widget_3);
        label_4->setObjectName("label_4");
        label_4->setMaximumSize(QSize(70, 25));

        gridLayout->addWidget(label_4, 1, 1, 1, 1);

        lblServer = new QLabel(widget_3);
        lblServer->setObjectName("lblServer");

        gridLayout->addWidget(lblServer, 0, 2, 1, 1);

        lblMap = new QLabel(widget_3);
        lblMap->setObjectName("lblMap");

        gridLayout->addWidget(lblMap, 1, 2, 1, 1);

        lblPlayers = new QLabel(widget_3);
        lblPlayers->setObjectName("lblPlayers");

        gridLayout->addWidget(lblPlayers, 2, 2, 1, 1);


        verticalLayout_3->addWidget(widget_3);

        splitter = new QSplitter(ServerConsoleDialog);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Horizontal);
        widget = new QWidget(splitter);
        widget->setObjectName("widget");
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, -1, -1);
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_3->addWidget(label_2);

        checkBox = new QCheckBox(widget);
        checkBox->setObjectName("checkBox");

        horizontalLayout_3->addWidget(checkBox);


        verticalLayout_2->addLayout(horizontalLayout_3);

        txtMainConsole = new QTextEdit(widget);
        txtMainConsole->setObjectName("txtMainConsole");
        txtMainConsole->setFrameShape(QFrame::Shape::StyledPanel);
        txtMainConsole->setLineWrapMode(QTextEdit::LineWrapMode::NoWrap);
        txtMainConsole->setTabStopDistance(80.000000000000000);
        txtMainConsole->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByKeyboard|Qt::TextInteractionFlag::TextSelectableByMouse);

        verticalLayout_2->addWidget(txtMainConsole);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, 5, -1, -1);
        lblInput = new QLabel(widget);
        lblInput->setObjectName("lblInput");

        horizontalLayout->addWidget(lblInput);

        lineInput = new QLineEdit(widget);
        lineInput->setObjectName("lineInput");

        horizontalLayout->addWidget(lineInput);


        verticalLayout_2->addLayout(horizontalLayout);

        splitter->addWidget(widget);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName("widget_2");
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName("verticalLayout");
        label_6 = new QLabel(widget_2);
        label_6->setObjectName("label_6");
        label_6->setMinimumSize(QSize(0, 29));

        verticalLayout->addWidget(label_6);

        txtChat = new QTextBrowser(widget_2);
        txtChat->setObjectName("txtChat");

        verticalLayout->addWidget(txtChat);

        label = new QLabel(widget_2);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        txtErrorConsole = new QTextBrowser(widget_2);
        txtErrorConsole->setObjectName("txtErrorConsole");

        verticalLayout->addWidget(txtErrorConsole);

        splitter->addWidget(widget_2);

        verticalLayout_3->addWidget(splitter);


        retranslateUi(ServerConsoleDialog);

        QMetaObject::connectSlotsByName(ServerConsoleDialog);
    } // setupUi

    void retranslateUi(QDialog *ServerConsoleDialog)
    {
        ServerConsoleDialog->setWindowTitle(QCoreApplication::translate("ServerConsoleDialog", "Dialog", nullptr));
        label_3->setText(QCoreApplication::translate("ServerConsoleDialog", "Server:", nullptr));
        label_5->setText(QCoreApplication::translate("ServerConsoleDialog", "Players:", nullptr));
        label_4->setText(QCoreApplication::translate("ServerConsoleDialog", "Map:", nullptr));
        lblServer->setText(QCoreApplication::translate("ServerConsoleDialog", "lblServer", nullptr));
        lblMap->setText(QCoreApplication::translate("ServerConsoleDialog", "lblMap", nullptr));
        lblPlayers->setText(QCoreApplication::translate("ServerConsoleDialog", "lblPlayers", nullptr));
        label_2->setText(QCoreApplication::translate("ServerConsoleDialog", "Main:", nullptr));
        checkBox->setText(QCoreApplication::translate("ServerConsoleDialog", "Wrap Text", nullptr));
        txtMainConsole->setHtml(QCoreApplication::translate("ServerConsoleDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Noto Sans'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        lblInput->setText(QCoreApplication::translate("ServerConsoleDialog", "Input:", nullptr));
        lineInput->setText(QString());
        label_6->setText(QCoreApplication::translate("ServerConsoleDialog", "Chat:", nullptr));
        label->setText(QCoreApplication::translate("ServerConsoleDialog", "Errors:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerConsoleDialog: public Ui_ServerConsoleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERCONSOLEDIALOG_H
