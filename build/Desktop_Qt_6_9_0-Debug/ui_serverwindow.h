/********************************************************************************
** Form generated from reading UI file 'serverwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWINDOW_H
#define UI_SERVERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerWindow
{
public:
    QFrame *frame;
    QWidget *PropsMain;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *lblPlayers;
    QLabel *lblFolder;
    QPushButton *btnSelectMap;
    QLabel *lblMap;
    QLabel *lblIP;
    QSpinBox *spinMaxPlayers;
    QLabel *lblParameters;
    QLineEdit *lineServerName;
    QLineEdit *lineMap;
    QLineEdit *lineParameters;
    QLineEdit *lineFolderName;
    QLabel *lblServerName;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineIP;
    QLabel *lblIPSplit;
    QLineEdit *linePort;
    QPushButton *btnCopyIp;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QPushButton *btnStartServer;
    QPushButton *btnInstallServer;
    QPushButton *btnStopServer;
    QCheckBox *chkConsole;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnConnectToServer;
    QPushButton *btnShowConsole;
    QPushButton *btnApply;
    QLabel *lblFolderError;
    QListWidget *listProps;
    QWidget *PropsConfigs;
    QLabel *label;

    void setupUi(QWidget *ServerWindow)
    {
        if (ServerWindow->objectName().isEmpty())
            ServerWindow->setObjectName("ServerWindow");
        ServerWindow->resize(800, 773);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(85, 40, 97, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        QBrush brush2(QColor(92, 92, 92, 255));
        brush2.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush2);
        QBrush brush3(QColor(127, 127, 127, 255));
        brush3.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush3);
        QBrush brush4(QColor(30, 30, 30, 255));
        brush4.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush4);
        QBrush brush5(QColor(18, 18, 18, 255));
        brush5.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Window, brush5);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Highlight, brush1);
        QBrush brush6(QColor(45, 45, 45, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush6);
        QBrush brush7(QColor(97, 97, 97, 255));
        brush7.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush7);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush5);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Highlight, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush7);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush3);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush6);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush7);
        ServerWindow->setPalette(palette);
        frame = new QFrame(ServerWindow);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 0, 631, 410));
        frame->setStyleSheet(QString::fromUtf8("border: 3px solid #552861;"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        PropsMain = new QWidget(ServerWindow);
        PropsMain->setObjectName("PropsMain");
        PropsMain->setEnabled(true);
        PropsMain->setGeometry(QRect(0, 0, 631, 411));
        gridLayoutWidget = new QWidget(PropsMain);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 100, 611, 301));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lblPlayers = new QLabel(gridLayoutWidget);
        lblPlayers->setObjectName("lblPlayers");

        gridLayout->addWidget(lblPlayers, 3, 0, 1, 1);

        lblFolder = new QLabel(gridLayoutWidget);
        lblFolder->setObjectName("lblFolder");

        gridLayout->addWidget(lblFolder, 1, 0, 1, 1);

        btnSelectMap = new QPushButton(gridLayoutWidget);
        btnSelectMap->setObjectName("btnSelectMap");

        gridLayout->addWidget(btnSelectMap, 4, 2, 1, 1);

        lblMap = new QLabel(gridLayoutWidget);
        lblMap->setObjectName("lblMap");

        gridLayout->addWidget(lblMap, 4, 0, 1, 1);

        lblIP = new QLabel(gridLayoutWidget);
        lblIP->setObjectName("lblIP");
        lblIP->setMaximumSize(QSize(16777215, 25));

        gridLayout->addWidget(lblIP, 2, 0, 1, 1);

        spinMaxPlayers = new QSpinBox(gridLayoutWidget);
        spinMaxPlayers->setObjectName("spinMaxPlayers");
        spinMaxPlayers->setMinimum(1);
        spinMaxPlayers->setMaximum(32);
        spinMaxPlayers->setValue(32);

        gridLayout->addWidget(spinMaxPlayers, 3, 1, 1, 1);

        lblParameters = new QLabel(gridLayoutWidget);
        lblParameters->setObjectName("lblParameters");

        gridLayout->addWidget(lblParameters, 5, 0, 1, 1);

        lineServerName = new QLineEdit(gridLayoutWidget);
        lineServerName->setObjectName("lineServerName");

        gridLayout->addWidget(lineServerName, 0, 1, 1, 1);

        lineMap = new QLineEdit(gridLayoutWidget);
        lineMap->setObjectName("lineMap");

        gridLayout->addWidget(lineMap, 4, 1, 1, 1);

        lineParameters = new QLineEdit(gridLayoutWidget);
        lineParameters->setObjectName("lineParameters");

        gridLayout->addWidget(lineParameters, 5, 1, 1, 1);

        lineFolderName = new QLineEdit(gridLayoutWidget);
        lineFolderName->setObjectName("lineFolderName");

        gridLayout->addWidget(lineFolderName, 1, 1, 1, 1);

        lblServerName = new QLabel(gridLayoutWidget);
        lblServerName->setObjectName("lblServerName");

        gridLayout->addWidget(lblServerName, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        lineIP = new QLineEdit(gridLayoutWidget);
        lineIP->setObjectName("lineIP");
        lineIP->setMinimumSize(QSize(0, 25));
        lineIP->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(lineIP);

        lblIPSplit = new QLabel(gridLayoutWidget);
        lblIPSplit->setObjectName("lblIPSplit");
        lblIPSplit->setMinimumSize(QSize(0, 25));
        lblIPSplit->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(lblIPSplit);

        linePort = new QLineEdit(gridLayoutWidget);
        linePort->setObjectName("linePort");
        linePort->setMinimumSize(QSize(80, 25));
        linePort->setMaximumSize(QSize(80, 25));

        horizontalLayout->addWidget(linePort);


        gridLayout->addLayout(horizontalLayout, 2, 1, 1, 1);

        btnCopyIp = new QPushButton(gridLayoutWidget);
        btnCopyIp->setObjectName("btnCopyIp");

        gridLayout->addWidget(btnCopyIp, 2, 2, 1, 1);

        gridLayoutWidget_2 = new QWidget(PropsMain);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(10, 10, 511, 89));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(2);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        btnStartServer = new QPushButton(gridLayoutWidget_2);
        btnStartServer->setObjectName("btnStartServer");
        btnStartServer->setMinimumSize(QSize(150, 50));
        btnStartServer->setMaximumSize(QSize(150, 50));

        gridLayout_2->addWidget(btnStartServer, 0, 1, 1, 1);

        btnInstallServer = new QPushButton(gridLayoutWidget_2);
        btnInstallServer->setObjectName("btnInstallServer");
        btnInstallServer->setMinimumSize(QSize(150, 50));
        btnInstallServer->setMaximumSize(QSize(150, 16777215));

        gridLayout_2->addWidget(btnInstallServer, 0, 0, 1, 1);

        btnStopServer = new QPushButton(gridLayoutWidget_2);
        btnStopServer->setObjectName("btnStopServer");
        btnStopServer->setEnabled(false);
        btnStopServer->setMinimumSize(QSize(150, 50));
        btnStopServer->setMaximumSize(QSize(150, 50));

        gridLayout_2->addWidget(btnStopServer, 0, 2, 1, 1);

        chkConsole = new QCheckBox(gridLayoutWidget_2);
        chkConsole->setObjectName("chkConsole");
        chkConsole->setEnabled(false);
        chkConsole->setChecked(true);

        gridLayout_2->addWidget(chkConsole, 1, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        btnConnectToServer = new QPushButton(gridLayoutWidget_2);
        btnConnectToServer->setObjectName("btnConnectToServer");
        btnConnectToServer->setEnabled(true);
        btnConnectToServer->setMinimumSize(QSize(100, 25));
        btnConnectToServer->setMaximumSize(QSize(100, 25));

        horizontalLayout_2->addWidget(btnConnectToServer);

        btnShowConsole = new QPushButton(gridLayoutWidget_2);
        btnShowConsole->setObjectName("btnShowConsole");
        btnShowConsole->setEnabled(false);
        btnShowConsole->setMinimumSize(QSize(42, 25));
        btnShowConsole->setMaximumSize(QSize(42, 25));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::WindowNew));
        btnShowConsole->setIcon(icon);

        horizontalLayout_2->addWidget(btnShowConsole);


        gridLayout_2->addLayout(horizontalLayout_2, 1, 1, 1, 1);

        btnApply = new QPushButton(PropsMain);
        btnApply->setObjectName("btnApply");
        btnApply->setEnabled(true);
        btnApply->setGeometry(QRect(530, 20, 91, 29));
        lblFolderError = new QLabel(PropsMain);
        lblFolderError->setObjectName("lblFolderError");
        lblFolderError->setGeometry(QRect(110, 140, 381, 21));
        QPalette palette1;
        QBrush brush8(QColor(223, 94, 96, 255));
        brush8.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush8);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush8);
        lblFolderError->setPalette(palette1);
        QFont font;
        font.setBold(false);
        font.setItalic(true);
        lblFolderError->setFont(font);
        listProps = new QListWidget(ServerWindow);
        QFont font1;
        font1.setPointSize(24);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listProps);
        __qlistwidgetitem->setTextAlignment(Qt::AlignCenter);
        __qlistwidgetitem->setFont(font1);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listProps);
        __qlistwidgetitem1->setTextAlignment(Qt::AlignCenter);
        __qlistwidgetitem1->setFont(font1);
        listProps->setObjectName("listProps");
        listProps->setGeometry(QRect(630, -1, 171, 421));
        QPalette palette2;
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Button, brush1);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Dark, brush2);
        QBrush brush9(QColor(127, 127, 127, 0));
        brush9.setStyle(Qt::BrushStyle::SolidPattern);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Mid, brush9);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Base, brush5);
        palette2.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Highlight, brush1);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush2);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush9);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush5);
        palette2.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Highlight, brush1);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush2);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush2);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush9);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush2);
        palette2.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush2);
        listProps->setPalette(palette2);
        listProps->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        listProps->setSpacing(0);
        listProps->setModelColumn(0);
        listProps->setSortingEnabled(false);
        PropsConfigs = new QWidget(ServerWindow);
        PropsConfigs->setObjectName("PropsConfigs");
        PropsConfigs->setGeometry(QRect(0, 410, 601, 411));
        label = new QLabel(PropsConfigs);
        label->setObjectName("label");
        label->setGeometry(QRect(120, 0, 361, 121));
        QFont font2;
        font2.setPointSize(36);
        label->setFont(font2);
        listProps->raise();
        frame->raise();
        PropsMain->raise();
        PropsConfigs->raise();

        retranslateUi(ServerWindow);

        listProps->setCurrentRow(0);


        QMetaObject::connectSlotsByName(ServerWindow);
    } // setupUi

    void retranslateUi(QWidget *ServerWindow)
    {
        ServerWindow->setWindowTitle(QCoreApplication::translate("ServerWindow", "Form", nullptr));
        lblPlayers->setText(QCoreApplication::translate("ServerWindow", "Players:", nullptr));
        lblFolder->setText(QCoreApplication::translate("ServerWindow", "Folder:", nullptr));
        btnSelectMap->setText(QCoreApplication::translate("ServerWindow", "Select", nullptr));
        lblMap->setText(QCoreApplication::translate("ServerWindow", "Map:", nullptr));
        lblIP->setText(QCoreApplication::translate("ServerWindow", "IP:", nullptr));
        lblParameters->setText(QCoreApplication::translate("ServerWindow", "Parameters:", nullptr));
        lblServerName->setText(QCoreApplication::translate("ServerWindow", "Server Name:", nullptr));
        lineIP->setText(QCoreApplication::translate("ServerWindow", "0.0.0.0", nullptr));
        lblIPSplit->setText(QCoreApplication::translate("ServerWindow", ":", nullptr));
        linePort->setText(QCoreApplication::translate("ServerWindow", "27015", nullptr));
        btnCopyIp->setText(QCoreApplication::translate("ServerWindow", "CopyIP", nullptr));
        btnStartServer->setText(QCoreApplication::translate("ServerWindow", "Start", nullptr));
        btnInstallServer->setText(QCoreApplication::translate("ServerWindow", "Install/Update", nullptr));
        btnStopServer->setText(QCoreApplication::translate("ServerWindow", "Stop", nullptr));
        chkConsole->setText(QCoreApplication::translate("ServerWindow", "OS Console", nullptr));
        btnConnectToServer->setText(QCoreApplication::translate("ServerWindow", "Connect", nullptr));
        btnShowConsole->setText(QString());
        btnApply->setText(QCoreApplication::translate("ServerWindow", "Apply", nullptr));
        lblFolderError->setText(QCoreApplication::translate("ServerWindow", "Folder name needs to be specified!", nullptr));

        const bool __sortingEnabled = listProps->isSortingEnabled();
        listProps->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listProps->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("ServerWindow", "Main", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listProps->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("ServerWindow", "Configs", nullptr));
        listProps->setSortingEnabled(__sortingEnabled);

        label->setText(QCoreApplication::translate("ServerWindow", "Configs", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ServerWindow: public Ui_ServerWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWINDOW_H
