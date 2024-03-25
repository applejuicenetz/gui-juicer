/********************************************************************************
** Form generated from reading UI file 'networkdialogbase.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORKDIALOGBASE_H
#define UI_NETWORKDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_NetworkDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *usersLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLabel *filesLabel;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLabel *sizeLabel;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLabel *ipLabel;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLabel *firewalledLabel;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *NetworkDialog)
    {
        if (NetworkDialog->objectName().isEmpty())
            NetworkDialog->setObjectName(QString::fromUtf8("NetworkDialog"));
        NetworkDialog->resize(507, 209);
        gridLayout = new QGridLayout(NetworkDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(NetworkDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(200, 0));
        label->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout->addWidget(label);

        usersLabel = new QLabel(NetworkDialog);
        usersLabel->setObjectName(QString::fromUtf8("usersLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(usersLabel->sizePolicy().hasHeightForWidth());
        usersLabel->setSizePolicy(sizePolicy1);
        usersLabel->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(usersLabel);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(NetworkDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(200, 0));
        label_2->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(label_2);

        filesLabel = new QLabel(NetworkDialog);
        filesLabel->setObjectName(QString::fromUtf8("filesLabel"));
        sizePolicy1.setHeightForWidth(filesLabel->sizePolicy().hasHeightForWidth());
        filesLabel->setSizePolicy(sizePolicy1);
        filesLabel->setMinimumSize(QSize(150, 0));

        horizontalLayout_2->addWidget(filesLabel);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(NetworkDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);
        label_3->setMinimumSize(QSize(200, 0));
        label_3->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_3->addWidget(label_3);

        sizeLabel = new QLabel(NetworkDialog);
        sizeLabel->setObjectName(QString::fromUtf8("sizeLabel"));
        sizePolicy1.setHeightForWidth(sizeLabel->sizePolicy().hasHeightForWidth());
        sizeLabel->setSizePolicy(sizePolicy1);
        sizeLabel->setMinimumSize(QSize(150, 0));

        horizontalLayout_3->addWidget(sizeLabel);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(NetworkDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);
        label_4->setMinimumSize(QSize(200, 0));
        label_4->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_4->addWidget(label_4);

        ipLabel = new QLabel(NetworkDialog);
        ipLabel->setObjectName(QString::fromUtf8("ipLabel"));
        sizePolicy1.setHeightForWidth(ipLabel->sizePolicy().hasHeightForWidth());
        ipLabel->setSizePolicy(sizePolicy1);
        ipLabel->setMinimumSize(QSize(150, 0));

        horizontalLayout_4->addWidget(ipLabel);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(NetworkDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy);
        label_5->setMinimumSize(QSize(200, 0));
        label_5->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_5->addWidget(label_5);

        firewalledLabel = new QLabel(NetworkDialog);
        firewalledLabel->setObjectName(QString::fromUtf8("firewalledLabel"));
        sizePolicy1.setHeightForWidth(firewalledLabel->sizePolicy().hasHeightForWidth());
        firewalledLabel->setSizePolicy(sizePolicy1);
        firewalledLabel->setMinimumSize(QSize(150, 0));

        horizontalLayout_5->addWidget(firewalledLabel);


        verticalLayout->addLayout(horizontalLayout_5);


        horizontalLayout_6->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_6, 0, 0, 1, 1);

        line = new QFrame(NetworkDialog);
        line->setObjectName(QString::fromUtf8("line"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy2);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(NetworkDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(NetworkDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), NetworkDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), NetworkDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(NetworkDialog);
    } // setupUi

    void retranslateUi(QDialog *NetworkDialog)
    {
        NetworkDialog->setWindowTitle(QApplication::translate("NetworkDialog", "Network Info", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("NetworkDialog", "Number of users:", 0, QApplication::UnicodeUTF8));
        usersLabel->setText(QApplication::translate("NetworkDialog", "0", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("NetworkDialog", "Number of files:", 0, QApplication::UnicodeUTF8));
        filesLabel->setText(QApplication::translate("NetworkDialog", "0", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("NetworkDialog", "Size of all files:", 0, QApplication::UnicodeUTF8));
        sizeLabel->setText(QApplication::translate("NetworkDialog", "0", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("NetworkDialog", "Your IP:", 0, QApplication::UnicodeUTF8));
        ipLabel->setText(QApplication::translate("NetworkDialog", "0", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("NetworkDialog", "Behind a firewall:", 0, QApplication::UnicodeUTF8));
        firewalledLabel->setText(QApplication::translate("NetworkDialog", "no", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NetworkDialog: public Ui_NetworkDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORKDIALOGBASE_H
