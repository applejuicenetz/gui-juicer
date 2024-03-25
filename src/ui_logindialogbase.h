/********************************************************************************
** Form generated from reading UI file 'logindialogbase.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOGBASE_H
#define UI_LOGINDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginDialogBase
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *headerLabel;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *hostEdit;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QSpinBox *portSpinBox;
    QToolButton *resetPortButton;
    QLabel *label_3;
    QLineEdit *passwordEdit;
    QCheckBox *savePasswordCheckBox;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LoginDialogBase)
    {
        if (LoginDialogBase->objectName().isEmpty())
            LoginDialogBase->setObjectName(QString::fromUtf8("LoginDialogBase"));
        LoginDialogBase->setWindowModality(Qt::ApplicationModal);
        LoginDialogBase->resize(266, 206);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LoginDialogBase->sizePolicy().hasHeightForWidth());
        LoginDialogBase->setSizePolicy(sizePolicy);
        LoginDialogBase->setModal(true);
        verticalLayout = new QVBoxLayout(LoginDialogBase);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        headerLabel = new QLabel(LoginDialogBase);
        headerLabel->setObjectName(QString::fromUtf8("headerLabel"));

        verticalLayout->addWidget(headerLabel);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label = new QLabel(LoginDialogBase);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        hostEdit = new QLineEdit(LoginDialogBase);
        hostEdit->setObjectName(QString::fromUtf8("hostEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(3);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(hostEdit->sizePolicy().hasHeightForWidth());
        hostEdit->setSizePolicy(sizePolicy2);

        formLayout->setWidget(0, QFormLayout::FieldRole, hostEdit);

        label_2 = new QLabel(LoginDialogBase);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy1);

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        portSpinBox = new QSpinBox(LoginDialogBase);
        portSpinBox->setObjectName(QString::fromUtf8("portSpinBox"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(3);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(portSpinBox->sizePolicy().hasHeightForWidth());
        portSpinBox->setSizePolicy(sizePolicy3);
        portSpinBox->setAlignment(Qt::AlignRight);
        portSpinBox->setMinimum(1);
        portSpinBox->setMaximum(65535);

        horizontalLayout->addWidget(portSpinBox);

        resetPortButton = new QToolButton(LoginDialogBase);
        resetPortButton->setObjectName(QString::fromUtf8("resetPortButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/small/undo.png"), QSize(), QIcon::Normal, QIcon::Off);
        resetPortButton->setIcon(icon);

        horizontalLayout->addWidget(resetPortButton);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout);

        label_3 = new QLabel(LoginDialogBase);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        passwordEdit = new QLineEdit(LoginDialogBase);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        sizePolicy2.setHeightForWidth(passwordEdit->sizePolicy().hasHeightForWidth());
        passwordEdit->setSizePolicy(sizePolicy2);
        passwordEdit->setEchoMode(QLineEdit::Password);

        formLayout->setWidget(2, QFormLayout::FieldRole, passwordEdit);


        verticalLayout->addLayout(formLayout);

        savePasswordCheckBox = new QCheckBox(LoginDialogBase);
        savePasswordCheckBox->setObjectName(QString::fromUtf8("savePasswordCheckBox"));
        savePasswordCheckBox->setLayoutDirection(Qt::LeftToRight);

        verticalLayout->addWidget(savePasswordCheckBox);

        line = new QFrame(LoginDialogBase);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        buttonBox = new QDialogButtonBox(LoginDialogBase);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(1);
        sizePolicy4.setVerticalStretch(1);
        sizePolicy4.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy4);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ignore|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        verticalLayout->addWidget(buttonBox);

        QWidget::setTabOrder(passwordEdit, savePasswordCheckBox);
        QWidget::setTabOrder(savePasswordCheckBox, buttonBox);
        QWidget::setTabOrder(buttonBox, hostEdit);
        QWidget::setTabOrder(hostEdit, portSpinBox);
        QWidget::setTabOrder(portSpinBox, resetPortButton);

        retranslateUi(LoginDialogBase);
        QObject::connect(buttonBox, SIGNAL(accepted()), LoginDialogBase, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LoginDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(LoginDialogBase);
    } // setupUi

    void retranslateUi(QDialog *LoginDialogBase)
    {
        LoginDialogBase->setWindowTitle(QApplication::translate("LoginDialogBase", "Juicer Login", 0, QApplication::UnicodeUTF8));
        headerLabel->setText(QApplication::translate("LoginDialogBase", "<h3>Login</h3>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("LoginDialogBase", "Host:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("LoginDialogBase", "Core-Port:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        resetPortButton->setToolTip(QApplication::translate("LoginDialogBase", "reset to default value", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        resetPortButton->setText(QApplication::translate("LoginDialogBase", "default value", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("LoginDialogBase", "Password:", 0, QApplication::UnicodeUTF8));
        savePasswordCheckBox->setText(QApplication::translate("LoginDialogBase", "save password", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LoginDialogBase: public Ui_LoginDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOGBASE_H
