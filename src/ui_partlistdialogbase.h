/********************************************************************************
** Form generated from reading UI file 'partlistdialogbase.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARTLISTDIALOGBASE_H
#define UI_PARTLISTDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "partswidget.h"

QT_BEGIN_NAMESPACE

class Ui_partListDialog
{
public:
    QHBoxLayout *horizontalLayout_6;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_7;
    PartsWidget *paintWidget;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *sizeIcon;
    QLabel *descrLabel;
    QLabel *sizeLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *readyIcon;
    QLabel *descrLabel_2;
    QLabel *readyLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *availableIcon;
    QLabel *descrLabel_3;
    QLabel *availableLabel;
    QHBoxLayout *horizontalLayout_4;
    QLabel *rareIcon;
    QLabel *descrLabel_4;
    QLabel *rareLabel;
    QHBoxLayout *horizontalLayout_5;
    QLabel *missingIcon;
    QLabel *descrLabel_5;
    QLabel *missingLabel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *partListDialog)
    {
        if (partListDialog->objectName().isEmpty())
            partListDialog->setObjectName(QString::fromUtf8("partListDialog"));
        partListDialog->resize(551, 209);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/partlist.png"), QSize(), QIcon::Normal, QIcon::Off);
        partListDialog->setWindowIcon(icon);
        partListDialog->setModal(false);
        horizontalLayout_6 = new QHBoxLayout(partListDialog);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        frame = new QFrame(partListDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(10);
        sizePolicy.setVerticalStretch(10);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setSizeIncrement(QSize(10, 10));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_7 = new QHBoxLayout(frame);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        paintWidget = new PartsWidget(frame);
        paintWidget->setObjectName(QString::fromUtf8("paintWidget"));

        horizontalLayout_7->addWidget(paintWidget);


        horizontalLayout_6->addWidget(frame);

        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        formLayout->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        sizeIcon = new QLabel(partListDialog);
        sizeIcon->setObjectName(QString::fromUtf8("sizeIcon"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sizeIcon->sizePolicy().hasHeightForWidth());
        sizeIcon->setSizePolicy(sizePolicy1);
        sizeIcon->setMinimumSize(QSize(20, 0));
        sizeIcon->setMaximumSize(QSize(20, 16777215));

        horizontalLayout->addWidget(sizeIcon);

        descrLabel = new QLabel(partListDialog);
        descrLabel->setObjectName(QString::fromUtf8("descrLabel"));

        horizontalLayout->addWidget(descrLabel);


        formLayout->setLayout(0, QFormLayout::LabelRole, horizontalLayout);

        sizeLabel = new QLabel(partListDialog);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));
        sizeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::FieldRole, sizeLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        readyIcon = new QLabel(partListDialog);
        readyIcon->setObjectName(QString::fromUtf8("readyIcon"));
        sizePolicy1.setHeightForWidth(readyIcon->sizePolicy().hasHeightForWidth());
        readyIcon->setSizePolicy(sizePolicy1);
        readyIcon->setMinimumSize(QSize(20, 0));
        readyIcon->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_2->addWidget(readyIcon);

        descrLabel_2 = new QLabel(partListDialog);
        descrLabel_2->setObjectName(QString::fromUtf8("descrLabel_2"));

        horizontalLayout_2->addWidget(descrLabel_2);


        formLayout->setLayout(1, QFormLayout::LabelRole, horizontalLayout_2);

        readyLabel = new QLabel(partListDialog);
        readyLabel->setObjectName(QString::fromUtf8("readyLabel"));
        readyLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::FieldRole, readyLabel);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        availableIcon = new QLabel(partListDialog);
        availableIcon->setObjectName(QString::fromUtf8("availableIcon"));
        sizePolicy1.setHeightForWidth(availableIcon->sizePolicy().hasHeightForWidth());
        availableIcon->setSizePolicy(sizePolicy1);
        availableIcon->setMinimumSize(QSize(20, 0));
        availableIcon->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_3->addWidget(availableIcon);

        descrLabel_3 = new QLabel(partListDialog);
        descrLabel_3->setObjectName(QString::fromUtf8("descrLabel_3"));

        horizontalLayout_3->addWidget(descrLabel_3);


        formLayout->setLayout(2, QFormLayout::LabelRole, horizontalLayout_3);

        availableLabel = new QLabel(partListDialog);
        availableLabel->setObjectName(QString::fromUtf8("availableLabel"));
        availableLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::FieldRole, availableLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        rareIcon = new QLabel(partListDialog);
        rareIcon->setObjectName(QString::fromUtf8("rareIcon"));
        sizePolicy1.setHeightForWidth(rareIcon->sizePolicy().hasHeightForWidth());
        rareIcon->setSizePolicy(sizePolicy1);
        rareIcon->setMinimumSize(QSize(20, 0));
        rareIcon->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_4->addWidget(rareIcon);

        descrLabel_4 = new QLabel(partListDialog);
        descrLabel_4->setObjectName(QString::fromUtf8("descrLabel_4"));

        horizontalLayout_4->addWidget(descrLabel_4);


        formLayout->setLayout(3, QFormLayout::LabelRole, horizontalLayout_4);

        rareLabel = new QLabel(partListDialog);
        rareLabel->setObjectName(QString::fromUtf8("rareLabel"));
        rareLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(3, QFormLayout::FieldRole, rareLabel);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        missingIcon = new QLabel(partListDialog);
        missingIcon->setObjectName(QString::fromUtf8("missingIcon"));
        sizePolicy1.setHeightForWidth(missingIcon->sizePolicy().hasHeightForWidth());
        missingIcon->setSizePolicy(sizePolicy1);
        missingIcon->setMinimumSize(QSize(20, 0));
        missingIcon->setMaximumSize(QSize(20, 16777215));

        horizontalLayout_5->addWidget(missingIcon);

        descrLabel_5 = new QLabel(partListDialog);
        descrLabel_5->setObjectName(QString::fromUtf8("descrLabel_5"));

        horizontalLayout_5->addWidget(descrLabel_5);


        formLayout->setLayout(4, QFormLayout::LabelRole, horizontalLayout_5);

        missingLabel = new QLabel(partListDialog);
        missingLabel->setObjectName(QString::fromUtf8("missingLabel"));
        missingLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(4, QFormLayout::FieldRole, missingLabel);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(partListDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy2);
        buttonBox->setStandardButtons(QDialogButtonBox::Close);

        verticalLayout->addWidget(buttonBox);


        horizontalLayout_6->addLayout(verticalLayout);


        retranslateUi(partListDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), partListDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), partListDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(partListDialog);
    } // setupUi

    void retranslateUi(QDialog *partListDialog)
    {
        partListDialog->setWindowTitle(QApplication::translate("partListDialog", "Parts", 0, QApplication::UnicodeUTF8));
        sizeIcon->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        descrLabel->setText(QApplication::translate("partListDialog", "size:", 0, QApplication::UnicodeUTF8));
        sizeLabel->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        readyIcon->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        descrLabel_2->setText(QApplication::translate("partListDialog", "ready:", 0, QApplication::UnicodeUTF8));
        readyLabel->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        availableIcon->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        descrLabel_3->setText(QApplication::translate("partListDialog", "available:", 0, QApplication::UnicodeUTF8));
        availableLabel->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        rareIcon->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        descrLabel_4->setText(QApplication::translate("partListDialog", "rare:", 0, QApplication::UnicodeUTF8));
        rareLabel->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        missingIcon->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        descrLabel_5->setText(QApplication::translate("partListDialog", "missing:", 0, QApplication::UnicodeUTF8));
        missingLabel->setText(QApplication::translate("partListDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class partListDialog: public Ui_partListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARTLISTDIALOGBASE_H
