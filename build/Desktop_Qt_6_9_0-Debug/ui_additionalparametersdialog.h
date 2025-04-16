/********************************************************************************
** Form generated from reading UI file 'additionalparametersdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDITIONALPARAMETERSDIALOG_H
#define UI_ADDITIONALPARAMETERSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AdditionalParametersDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAddParameter;
    QPushButton *btnRemoveParameter;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QTreeWidget *treeParameters;

    void setupUi(QDialog *AdditionalParametersDialog)
    {
        if (AdditionalParametersDialog->objectName().isEmpty())
            AdditionalParametersDialog->setObjectName("AdditionalParametersDialog");
        AdditionalParametersDialog->resize(553, 427);
        verticalLayout = new QVBoxLayout(AdditionalParametersDialog);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnAddParameter = new QPushButton(AdditionalParametersDialog);
        btnAddParameter->setObjectName("btnAddParameter");
        btnAddParameter->setMinimumSize(QSize(32, 32));
        btnAddParameter->setMaximumSize(QSize(32, 32));
        btnAddParameter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	border: none;\n"
"	border-bottom: 3px solid #00ff00;\n"
"	background-color: #303030;\n"
"}\n"
"\n"
"QPushButton::hover {\n"
"	background-color: #00c200;\n"
"}\n"
"QPushButton::pressed {\n"
"	background-color: #00b300;\n"
"}"));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd));
        btnAddParameter->setIcon(icon);
        btnAddParameter->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(btnAddParameter);

        btnRemoveParameter = new QPushButton(AdditionalParametersDialog);
        btnRemoveParameter->setObjectName("btnRemoveParameter");
        btnRemoveParameter->setMinimumSize(QSize(32, 32));
        btnRemoveParameter->setMaximumSize(QSize(32, 32));
        btnRemoveParameter->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	border: none;\n"
"	border-bottom: 3px solid #ff0000;\n"
"	background-color: #303030;\n"
"}\n"
"\n"
"QPushButton::hover {\n"
"	background-color: #c20000;\n"
"}\n"
"QPushButton::pressed {\n"
"	background-color: #b30000;\n"
"}"));
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));
        btnRemoveParameter->setIcon(icon1);
        btnRemoveParameter->setIconSize(QSize(16, 16));

        horizontalLayout->addWidget(btnRemoveParameter);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(AdditionalParametersDialog);
        label->setObjectName("label");
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::WindowText, brush);
        QBrush brush1(QColor(207, 106, 50, 255));
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
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::Link, brush1);
        QBrush brush6(QColor(255, 158, 105, 255));
        brush6.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::LinkVisited, brush6);
        QBrush brush7(QColor(45, 45, 45, 255));
        brush7.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::AlternateBase, brush7);
        QBrush brush8(QColor(97, 97, 97, 255));
        brush8.setStyle(Qt::BrushStyle::SolidPattern);
        palette.setBrush(QPalette::ColorGroup::Active, QPalette::ColorRole::ToolTipBase, brush8);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::WindowText, brush);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Dark, brush2);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Mid, brush3);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Base, brush4);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Window, brush5);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Highlight, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::Link, brush1);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::LinkVisited, brush6);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::AlternateBase, brush7);
        palette.setBrush(QPalette::ColorGroup::Inactive, QPalette::ColorRole::ToolTipBase, brush8);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::WindowText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Button, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Dark, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Mid, brush3);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Text, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ButtonText, brush2);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Window, brush5);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Link, brush1);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::LinkVisited, brush6);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::AlternateBase, brush7);
        palette.setBrush(QPalette::ColorGroup::Disabled, QPalette::ColorRole::ToolTipBase, brush8);
        label->setPalette(palette);
        label->setStyleSheet(QString::fromUtf8(""));
        label->setTextFormat(Qt::TextFormat::RichText);
        label->setOpenExternalLinks(true);
        label->setTextInteractionFlags(Qt::TextInteractionFlag::TextBrowserInteraction);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        treeParameters = new QTreeWidget(AdditionalParametersDialog);
        treeParameters->setObjectName("treeParameters");
        treeParameters->setProperty("showDropIndicator", QVariant(false));
        treeParameters->setIndentation(0);
        treeParameters->setRootIsDecorated(false);
        treeParameters->setItemsExpandable(false);
        treeParameters->setExpandsOnDoubleClick(false);

        verticalLayout->addWidget(treeParameters);


        retranslateUi(AdditionalParametersDialog);

        QMetaObject::connectSlotsByName(AdditionalParametersDialog);
    } // setupUi

    void retranslateUi(QDialog *AdditionalParametersDialog)
    {
        AdditionalParametersDialog->setWindowTitle(QCoreApplication::translate("AdditionalParametersDialog", "Dialog", nullptr));
        btnAddParameter->setText(QString());
        btnRemoveParameter->setText(QString());
        label->setText(QCoreApplication::translate("AdditionalParametersDialog", "<a href=\"https://developer.valvesoftware.com/wiki/Command_line_options#Source_Dedicated_Server\">Command Line Parameters</a>", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = treeParameters->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("AdditionalParametersDialog", "Enabled", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("AdditionalParametersDialog", "Value", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("AdditionalParametersDialog", "Parameter", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdditionalParametersDialog: public Ui_AdditionalParametersDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDITIONALPARAMETERSDIALOG_H
