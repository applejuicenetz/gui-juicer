/********************************************************************************
** Form generated from reading ui file 'qajoptionsdialogbase.ui'
**
** Created: Sat Apr 7 00:31:10 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_QAJOPTIONSDIALOGBASE_H
#define UI_QAJOPTIONSDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_QAjOptionsDialogBase
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *widget;
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *passwordEdit;
    QLabel *label_7;
    QLabel *label_9;
    QToolButton *tempButton;
    QLineEdit *nickEdit;
    QLineEdit *tempEdit;
    QLineEdit *serverEdit;
    QLineEdit *coreEdit;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_4;
    QLineEdit *retypeEdit;
    QLabel *label_3;
    QToolButton *incomingButton;
    QLabel *label_8;
    QSpinBox *refreshSpin;
    QLineEdit *xmlEdit;
    QLineEdit *incomingEdit;
    QLabel *label_5;
    QHBoxLayout *hboxLayout1;
    QCheckBox *autoConnect;
    QSpacerItem *spacerItem1;
    QCheckBox *savePassword;
    QWidget *tab_2;
    QWidget *widget1;
    QGridLayout *gridLayout1;
    QSpinBox *slotSpin;
    QSpinBox *sourcesSpin;
    QLabel *label_15;
    QSpinBox *connectionsSpin;
    QLabel *label_11;
    QLabel *label_10;
    QLineEdit *tcpEdit;
    QSpinBox *upSpin;
    QLabel *label_12;
    QLabel *label_16;
    QSpinBox *newSpin;
    QLabel *label_14;
    QLabel *label_13;
    QSpinBox *downSpin;
    QWidget *ftp_tab;
    QWidget *widget2;
    QGridLayout *gridLayout2;
    QLineEdit *ftpServerEdit;
    QLabel *label_18;
    QLabel *label_19;
    QLineEdit *ftpPasswordEdit;
    QLabel *label_17;
    QLineEdit *ftpUserEdit;
    QLabel *label_20;
    QLineEdit *ftpPortEdit;
    QLineEdit *ftpDirEdit;
    QLabel *label_21;

    void setupUi(QDialog *QAjOptionsDialogBase)
    {
    QAjOptionsDialogBase->setObjectName(QString::fromUtf8("QAjOptionsDialogBase"));
    layoutWidget = new QWidget(QAjOptionsDialogBase);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(20, 390, 381, 33));
    hboxLayout = new QHBoxLayout(layoutWidget);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    okButton = new QPushButton(layoutWidget);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    hboxLayout->addWidget(okButton);

    cancelButton = new QPushButton(layoutWidget);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout->addWidget(cancelButton);

    tabWidget = new QTabWidget(QAjOptionsDialogBase);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setEnabled(true);
    tabWidget->setGeometry(QRect(20, 20, 381, 351));
    tab = new QWidget();
    tab->setObjectName(QString::fromUtf8("tab"));
    widget = new QWidget(tab);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(10, 10, 351, 307));
    vboxLayout = new QVBoxLayout(widget);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(widget);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout->addWidget(label, 0, 0, 1, 1);

    passwordEdit = new QLineEdit(widget);
    passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));

    gridLayout->addWidget(passwordEdit, 1, 1, 1, 1);

    label_7 = new QLabel(widget);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout->addWidget(label_7, 6, 0, 1, 1);

    label_9 = new QLabel(widget);
    label_9->setObjectName(QString::fromUtf8("label_9"));

    gridLayout->addWidget(label_9, 8, 0, 1, 1);

    tempButton = new QToolButton(widget);
    tempButton->setObjectName(QString::fromUtf8("tempButton"));

    gridLayout->addWidget(tempButton, 6, 2, 1, 1);

    nickEdit = new QLineEdit(widget);
    nickEdit->setObjectName(QString::fromUtf8("nickEdit"));

    gridLayout->addWidget(nickEdit, 0, 1, 1, 1);

    tempEdit = new QLineEdit(widget);
    tempEdit->setObjectName(QString::fromUtf8("tempEdit"));

    gridLayout->addWidget(tempEdit, 6, 1, 1, 1);

    serverEdit = new QLineEdit(widget);
    serverEdit->setObjectName(QString::fromUtf8("serverEdit"));

    gridLayout->addWidget(serverEdit, 7, 1, 1, 1);

    coreEdit = new QLineEdit(widget);
    coreEdit->setObjectName(QString::fromUtf8("coreEdit"));

    gridLayout->addWidget(coreEdit, 3, 1, 1, 1);

    label_6 = new QLabel(widget);
    label_6->setObjectName(QString::fromUtf8("label_6"));

    gridLayout->addWidget(label_6, 5, 0, 1, 1);

    label_2 = new QLabel(widget);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    label_4 = new QLabel(widget);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout->addWidget(label_4, 3, 0, 1, 1);

    retypeEdit = new QLineEdit(widget);
    retypeEdit->setObjectName(QString::fromUtf8("retypeEdit"));

    gridLayout->addWidget(retypeEdit, 2, 1, 1, 1);

    label_3 = new QLabel(widget);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    incomingButton = new QToolButton(widget);
    incomingButton->setObjectName(QString::fromUtf8("incomingButton"));

    gridLayout->addWidget(incomingButton, 5, 2, 1, 1);

    label_8 = new QLabel(widget);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout->addWidget(label_8, 7, 0, 1, 1);

    refreshSpin = new QSpinBox(widget);
    refreshSpin->setObjectName(QString::fromUtf8("refreshSpin"));
    refreshSpin->setMaximumSize(QSize(100, 16777215));
    refreshSpin->setMinimum(1);

    gridLayout->addWidget(refreshSpin, 8, 1, 1, 1);

    xmlEdit = new QLineEdit(widget);
    xmlEdit->setObjectName(QString::fromUtf8("xmlEdit"));

    gridLayout->addWidget(xmlEdit, 4, 1, 1, 1);

    incomingEdit = new QLineEdit(widget);
    incomingEdit->setObjectName(QString::fromUtf8("incomingEdit"));

    gridLayout->addWidget(incomingEdit, 5, 1, 1, 1);

    label_5 = new QLabel(widget);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout->addWidget(label_5, 4, 0, 1, 1);


    vboxLayout->addLayout(gridLayout);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    autoConnect = new QCheckBox(widget);
    autoConnect->setObjectName(QString::fromUtf8("autoConnect"));

    hboxLayout1->addWidget(autoConnect);

    spacerItem1 = new QSpacerItem(61, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);

    savePassword = new QCheckBox(widget);
    savePassword->setObjectName(QString::fromUtf8("savePassword"));

    hboxLayout1->addWidget(savePassword);


    vboxLayout->addLayout(hboxLayout1);

    tabWidget->addTab(tab, QApplication::translate("QAjOptionsDialogBase", "Common", 0, QApplication::UnicodeUTF8));
    tab_2 = new QWidget();
    tab_2->setObjectName(QString::fromUtf8("tab_2"));
    widget1 = new QWidget(tab_2);
    widget1->setObjectName(QString::fromUtf8("widget1"));
    widget1->setGeometry(QRect(10, 10, 351, 213));
    gridLayout1 = new QGridLayout(widget1);
    gridLayout1->setSpacing(6);
    gridLayout1->setMargin(0);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    slotSpin = new QSpinBox(widget1);
    slotSpin->setObjectName(QString::fromUtf8("slotSpin"));
    slotSpin->setAlignment(Qt::AlignRight);
    slotSpin->setMaximum(9999);

    gridLayout1->addWidget(slotSpin, 2, 1, 1, 1);

    sourcesSpin = new QSpinBox(widget1);
    sourcesSpin->setObjectName(QString::fromUtf8("sourcesSpin"));
    sourcesSpin->setAlignment(Qt::AlignRight);
    sourcesSpin->setMaximum(9999);

    gridLayout1->addWidget(sourcesSpin, 3, 1, 1, 1);

    label_15 = new QLabel(widget1);
    label_15->setObjectName(QString::fromUtf8("label_15"));

    gridLayout1->addWidget(label_15, 5, 0, 1, 1);

    connectionsSpin = new QSpinBox(widget1);
    connectionsSpin->setObjectName(QString::fromUtf8("connectionsSpin"));
    connectionsSpin->setAlignment(Qt::AlignRight);
    connectionsSpin->setMaximum(9999);

    gridLayout1->addWidget(connectionsSpin, 4, 1, 1, 1);

    label_11 = new QLabel(widget1);
    label_11->setObjectName(QString::fromUtf8("label_11"));

    gridLayout1->addWidget(label_11, 1, 0, 1, 1);

    label_10 = new QLabel(widget1);
    label_10->setObjectName(QString::fromUtf8("label_10"));

    gridLayout1->addWidget(label_10, 0, 0, 1, 1);

    tcpEdit = new QLineEdit(widget1);
    tcpEdit->setObjectName(QString::fromUtf8("tcpEdit"));

    gridLayout1->addWidget(tcpEdit, 6, 1, 1, 1);

    upSpin = new QSpinBox(widget1);
    upSpin->setObjectName(QString::fromUtf8("upSpin"));
    upSpin->setAlignment(Qt::AlignRight);
    upSpin->setMaximum(9999);

    gridLayout1->addWidget(upSpin, 1, 1, 1, 1);

    label_12 = new QLabel(widget1);
    label_12->setObjectName(QString::fromUtf8("label_12"));

    gridLayout1->addWidget(label_12, 2, 0, 1, 1);

    label_16 = new QLabel(widget1);
    label_16->setObjectName(QString::fromUtf8("label_16"));

    gridLayout1->addWidget(label_16, 6, 0, 1, 1);

    newSpin = new QSpinBox(widget1);
    newSpin->setObjectName(QString::fromUtf8("newSpin"));
    newSpin->setAlignment(Qt::AlignRight);
    newSpin->setMaximum(9999);

    gridLayout1->addWidget(newSpin, 5, 1, 1, 1);

    label_14 = new QLabel(widget1);
    label_14->setObjectName(QString::fromUtf8("label_14"));

    gridLayout1->addWidget(label_14, 4, 0, 1, 1);

    label_13 = new QLabel(widget1);
    label_13->setObjectName(QString::fromUtf8("label_13"));

    gridLayout1->addWidget(label_13, 3, 0, 1, 1);

    downSpin = new QSpinBox(widget1);
    downSpin->setObjectName(QString::fromUtf8("downSpin"));
    downSpin->setAlignment(Qt::AlignRight);
    downSpin->setMaximum(9999);

    gridLayout1->addWidget(downSpin, 0, 1, 1, 1);

    tabWidget->addTab(tab_2, QApplication::translate("QAjOptionsDialogBase", "Network", 0, QApplication::UnicodeUTF8));
    ftp_tab = new QWidget();
    ftp_tab->setObjectName(QString::fromUtf8("ftp_tab"));
    widget2 = new QWidget(ftp_tab);
    widget2->setObjectName(QString::fromUtf8("widget2"));
    widget2->setGeometry(QRect(10, 10, 351, 151));
    gridLayout2 = new QGridLayout(widget2);
    gridLayout2->setSpacing(6);
    gridLayout2->setMargin(0);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    ftpServerEdit = new QLineEdit(widget2);
    ftpServerEdit->setObjectName(QString::fromUtf8("ftpServerEdit"));

    gridLayout2->addWidget(ftpServerEdit, 0, 1, 1, 1);

    label_18 = new QLabel(widget2);
    label_18->setObjectName(QString::fromUtf8("label_18"));

    gridLayout2->addWidget(label_18, 1, 0, 1, 1);

    label_19 = new QLabel(widget2);
    label_19->setObjectName(QString::fromUtf8("label_19"));

    gridLayout2->addWidget(label_19, 2, 0, 1, 1);

    ftpPasswordEdit = new QLineEdit(widget2);
    ftpPasswordEdit->setObjectName(QString::fromUtf8("ftpPasswordEdit"));
    ftpPasswordEdit->setEchoMode(QLineEdit::Password);

    gridLayout2->addWidget(ftpPasswordEdit, 3, 1, 1, 1);

    label_17 = new QLabel(widget2);
    label_17->setObjectName(QString::fromUtf8("label_17"));

    gridLayout2->addWidget(label_17, 0, 0, 1, 1);

    ftpUserEdit = new QLineEdit(widget2);
    ftpUserEdit->setObjectName(QString::fromUtf8("ftpUserEdit"));

    gridLayout2->addWidget(ftpUserEdit, 2, 1, 1, 1);

    label_20 = new QLabel(widget2);
    label_20->setObjectName(QString::fromUtf8("label_20"));

    gridLayout2->addWidget(label_20, 3, 0, 1, 1);

    ftpPortEdit = new QLineEdit(widget2);
    ftpPortEdit->setObjectName(QString::fromUtf8("ftpPortEdit"));

    gridLayout2->addWidget(ftpPortEdit, 1, 1, 1, 1);

    ftpDirEdit = new QLineEdit(widget2);
    ftpDirEdit->setObjectName(QString::fromUtf8("ftpDirEdit"));

    gridLayout2->addWidget(ftpDirEdit, 4, 1, 1, 1);

    label_21 = new QLabel(widget2);
    label_21->setObjectName(QString::fromUtf8("label_21"));

    gridLayout2->addWidget(label_21, 4, 0, 1, 1);

    tabWidget->addTab(ftp_tab, QApplication::translate("QAjOptionsDialogBase", "Ftp", 0, QApplication::UnicodeUTF8));
    QWidget::setTabOrder(okButton, cancelButton);
    QWidget::setTabOrder(cancelButton, tabWidget);
    QWidget::setTabOrder(tabWidget, nickEdit);
    QWidget::setTabOrder(nickEdit, passwordEdit);
    QWidget::setTabOrder(passwordEdit, retypeEdit);
    QWidget::setTabOrder(retypeEdit, coreEdit);
    QWidget::setTabOrder(coreEdit, xmlEdit);
    QWidget::setTabOrder(xmlEdit, incomingEdit);
    QWidget::setTabOrder(incomingEdit, incomingButton);
    QWidget::setTabOrder(incomingButton, tempEdit);
    QWidget::setTabOrder(tempEdit, tempButton);
    QWidget::setTabOrder(tempButton, serverEdit);
    QWidget::setTabOrder(serverEdit, refreshSpin);
    QWidget::setTabOrder(refreshSpin, autoConnect);
    QWidget::setTabOrder(autoConnect, savePassword);
    QWidget::setTabOrder(savePassword, downSpin);
    QWidget::setTabOrder(downSpin, upSpin);
    QWidget::setTabOrder(upSpin, slotSpin);
    QWidget::setTabOrder(slotSpin, sourcesSpin);
    QWidget::setTabOrder(sourcesSpin, connectionsSpin);
    QWidget::setTabOrder(connectionsSpin, newSpin);
    QWidget::setTabOrder(newSpin, tcpEdit);
    QWidget::setTabOrder(tcpEdit, ftpServerEdit);
    QWidget::setTabOrder(ftpServerEdit, ftpPortEdit);
    QWidget::setTabOrder(ftpPortEdit, ftpUserEdit);
    QWidget::setTabOrder(ftpUserEdit, ftpPasswordEdit);
    QWidget::setTabOrder(ftpPasswordEdit, ftpDirEdit);

    retranslateUi(QAjOptionsDialogBase);

    QSize size(421, 440);
    size = size.expandedTo(QAjOptionsDialogBase->minimumSizeHint());
    QAjOptionsDialogBase->resize(size);

    QObject::connect(okButton, SIGNAL(clicked()), QAjOptionsDialogBase, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), QAjOptionsDialogBase, SLOT(reject()));

    QMetaObject::connectSlotsByName(QAjOptionsDialogBase);
    } // setupUi

    void retranslateUi(QDialog *QAjOptionsDialogBase)
    {
    QAjOptionsDialogBase->setWindowTitle(QApplication::translate("QAjOptionsDialogBase", "Configure", 0, QApplication::UnicodeUTF8));
    okButton->setText(QApplication::translate("QAjOptionsDialogBase", "OK", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("QAjOptionsDialogBase", "Cancel", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("QAjOptionsDialogBase", "Nick:", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("QAjOptionsDialogBase", "Temporary Directory:", 0, QApplication::UnicodeUTF8));
    label_9->setText(QApplication::translate("QAjOptionsDialogBase", "Refresh:", 0, QApplication::UnicodeUTF8));
    tempButton->setText(QApplication::translate("QAjOptionsDialogBase", "...", 0, QApplication::UnicodeUTF8));
    label_6->setText(QApplication::translate("QAjOptionsDialogBase", "Incoming Directory:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("QAjOptionsDialogBase", "New Password:", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("QAjOptionsDialogBase", "Core Address:", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("QAjOptionsDialogBase", "Retype Password:", 0, QApplication::UnicodeUTF8));
    incomingButton->setText(QApplication::translate("QAjOptionsDialogBase", "...", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("QAjOptionsDialogBase", "Server Source:", 0, QApplication::UnicodeUTF8));
    refreshSpin->setSuffix(QApplication::translate("QAjOptionsDialogBase", "sec.", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("QAjOptionsDialogBase", "XML Port:", 0, QApplication::UnicodeUTF8));
    autoConnect->setText(QApplication::translate("QAjOptionsDialogBase", "auto connect", 0, QApplication::UnicodeUTF8));
    savePassword->setText(QApplication::translate("QAjOptionsDialogBase", "save password", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("QAjOptionsDialogBase", "Common", 0, QApplication::UnicodeUTF8));
    slotSpin->setSuffix(QApplication::translate("QAjOptionsDialogBase", " kByte/s", 0, QApplication::UnicodeUTF8));
    sourcesSpin->setSuffix(QString());
    label_15->setText(QApplication::translate("QAjOptionsDialogBase", "Max. New Connections:", 0, QApplication::UnicodeUTF8));
    connectionsSpin->setSuffix(QString());
    label_11->setText(QApplication::translate("QAjOptionsDialogBase", "Max. Upload Speed:", 0, QApplication::UnicodeUTF8));
    label_10->setText(QApplication::translate("QAjOptionsDialogBase", "Max. Download Speed:", 0, QApplication::UnicodeUTF8));
    upSpin->setSuffix(QApplication::translate("QAjOptionsDialogBase", " kByte/s", 0, QApplication::UnicodeUTF8));
    label_12->setText(QApplication::translate("QAjOptionsDialogBase", "Max. Speed Per Slot:", 0, QApplication::UnicodeUTF8));
    label_16->setText(QApplication::translate("QAjOptionsDialogBase", "TCP Port:", 0, QApplication::UnicodeUTF8));
    newSpin->setSuffix(QApplication::translate("QAjOptionsDialogBase", " per 10 seconds", 0, QApplication::UnicodeUTF8));
    label_14->setText(QApplication::translate("QAjOptionsDialogBase", "Max. Connections:", 0, QApplication::UnicodeUTF8));
    label_13->setText(QApplication::translate("QAjOptionsDialogBase", "Max. Sources Per File:", 0, QApplication::UnicodeUTF8));
    downSpin->setSuffix(QApplication::translate("QAjOptionsDialogBase", " kByte/s", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QAjOptionsDialogBase", "Network", 0, QApplication::UnicodeUTF8));
    label_18->setText(QApplication::translate("QAjOptionsDialogBase", "Port:", 0, QApplication::UnicodeUTF8));
    label_19->setText(QApplication::translate("QAjOptionsDialogBase", "User:", 0, QApplication::UnicodeUTF8));
    label_17->setText(QApplication::translate("QAjOptionsDialogBase", "Server:", 0, QApplication::UnicodeUTF8));
    label_20->setText(QApplication::translate("QAjOptionsDialogBase", "Password:", 0, QApplication::UnicodeUTF8));
    label_21->setText(QApplication::translate("QAjOptionsDialogBase", "Directory:", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabText(tabWidget->indexOf(ftp_tab), QApplication::translate("QAjOptionsDialogBase", "Ftp", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(QAjOptionsDialogBase);
    } // retranslateUi

};

namespace Ui {
    class QAjOptionsDialogBase: public Ui_QAjOptionsDialogBase {};
} // namespace Ui

#endif // UI_QAJOPTIONSDIALOGBASE_H
