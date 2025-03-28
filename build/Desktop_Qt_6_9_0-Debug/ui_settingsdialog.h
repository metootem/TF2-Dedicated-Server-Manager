/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *btnSrvDir;
    QLabel *lblSrvDir;
    QLineEdit *lineSrvDir;
    QPushButton *btnApply;
    QLabel *lblSrvDirError;
    QLabel *lblApplySuccess;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName("SettingsDialog");
        SettingsDialog->resize(800, 125);
        SettingsDialog->setMinimumSize(QSize(800, 125));
        SettingsDialog->setMaximumSize(QSize(800, 125));
        gridLayoutWidget = new QWidget(SettingsDialog);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(10, 10, 781, 101));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        btnSrvDir = new QPushButton(gridLayoutWidget);
        btnSrvDir->setObjectName("btnSrvDir");

        gridLayout->addWidget(btnSrvDir, 0, 2, 1, 1);

        lblSrvDir = new QLabel(gridLayoutWidget);
        lblSrvDir->setObjectName("lblSrvDir");

        gridLayout->addWidget(lblSrvDir, 0, 0, 1, 1);

        lineSrvDir = new QLineEdit(gridLayoutWidget);
        lineSrvDir->setObjectName("lineSrvDir");

        gridLayout->addWidget(lineSrvDir, 0, 1, 1, 1);

        btnApply = new QPushButton(gridLayoutWidget);
        btnApply->setObjectName("btnApply");

        gridLayout->addWidget(btnApply, 1, 2, 1, 1);

        lblSrvDirError = new QLabel(SettingsDialog);
        lblSrvDirError->setObjectName("lblSrvDirError");
        lblSrvDirError->setEnabled(true);
        lblSrvDirError->setGeometry(QRect(145, 2, 351, 20));
        QPalette palette;
        QBrush brush(QColor(223, 94, 96, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        lblSrvDirError->setPalette(palette);
        QFont font;
        font.setBold(false);
        font.setItalic(true);
        lblSrvDirError->setFont(font);
        lblApplySuccess = new QLabel(SettingsDialog);
        lblApplySuccess->setObjectName("lblApplySuccess");
        lblApplySuccess->setEnabled(true);
        lblApplySuccess->setGeometry(QRect(670, 100, 131, 21));
        QPalette palette1;
        QBrush brush1(QColor(84, 223, 89, 255));
        brush1.setStyle(Qt::BrushStyle::SolidPattern);
        palette1.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush1);
        palette1.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush1);
        lblApplySuccess->setPalette(palette1);
        lblApplySuccess->setFont(font);

        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "TF2-DSM Settings", nullptr));
        btnSrvDir->setText(QCoreApplication::translate("SettingsDialog", "Browse", nullptr));
        lblSrvDir->setText(QCoreApplication::translate("SettingsDialog", "Servers Directory:", nullptr));
        lineSrvDir->setText(QString());
        btnApply->setText(QCoreApplication::translate("SettingsDialog", "Apply", nullptr));
        lblSrvDirError->setText(QCoreApplication::translate("SettingsDialog", "Invalid directory!", nullptr));
        lblApplySuccess->setText(QCoreApplication::translate("SettingsDialog", "Settings applied!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
