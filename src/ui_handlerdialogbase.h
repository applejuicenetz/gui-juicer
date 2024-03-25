/********************************************************************************
** Form generated from reading UI file 'handlerdialogbase.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HANDLERDIALOGBASE_H
#define UI_HANDLERDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HandlerDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *iconLabel;
    QSpacerItem *verticalSpacer;
    QLabel *textLabel;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *chkAskAgain;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *HandlerDialog)
    {
        if (HandlerDialog->objectName().isEmpty())
            HandlerDialog->setObjectName(QString::fromUtf8("HandlerDialog"));
        HandlerDialog->resize(400, 200);
        HandlerDialog->setMinimumSize(QSize(400, 200));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/juicer.png"), QSize(), QIcon::Normal, QIcon::Off);
        HandlerDialog->setWindowIcon(icon);
        verticalLayout_2 = new QVBoxLayout(HandlerDialog);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        iconLabel = new QLabel(HandlerDialog);
        iconLabel->setObjectName(QString::fromUtf8("iconLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(iconLabel->sizePolicy().hasHeightForWidth());
        iconLabel->setSizePolicy(sizePolicy);
        iconLabel->setMinimumSize(QSize(50, 50));
        iconLabel->setMaximumSize(QSize(50, 50));
        iconLabel->setPixmap(QPixmap(QString::fromUtf8(":/juicer.png")));

        verticalLayout->addWidget(iconLabel);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        textLabel = new QLabel(HandlerDialog);
        textLabel->setObjectName(QString::fromUtf8("textLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(textLabel->sizePolicy().hasHeightForWidth());
        textLabel->setSizePolicy(sizePolicy1);
        textLabel->setSizeIncrement(QSize(1, 1));
        textLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        textLabel->setWordWrap(true);

        horizontalLayout->addWidget(textLabel);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(10, -1, -1, -1);
        chkAskAgain = new QCheckBox(HandlerDialog);
        chkAskAgain->setObjectName(QString::fromUtf8("chkAskAgain"));

        horizontalLayout_2->addWidget(chkAskAgain);

        buttonBox = new QDialogButtonBox(HandlerDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(HandlerDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), HandlerDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), HandlerDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(HandlerDialog);
    } // setupUi

    void retranslateUi(QDialog *HandlerDialog)
    {
        HandlerDialog->setWindowTitle(QApplication::translate("HandlerDialog", "Caption", 0, QApplication::UnicodeUTF8));
        iconLabel->setText(QString());
        textLabel->setText(QApplication::translate("HandlerDialog", "Text", 0, QApplication::UnicodeUTF8));
        chkAskAgain->setText(QApplication::translate("HandlerDialog", "Don't ask me again.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HandlerDialog: public Ui_HandlerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HANDLERDIALOGBASE_H
