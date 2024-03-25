/********************************************************************************
** Form generated from reading UI file 'dirselectiondialogbase.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIRSELECTIONDIALOGBASE_H
#define UI_DIRSELECTIONDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DirSelectionDialogBase
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *textLabel;
    QTreeWidget *treeWidget;
    QLabel *labelNewFolder;
    QLineEdit *leNewFolder;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DirSelectionDialogBase)
    {
        if (DirSelectionDialogBase->objectName().isEmpty())
            DirSelectionDialogBase->setObjectName(QString::fromUtf8("DirSelectionDialogBase"));
        DirSelectionDialogBase->resize(500, 300);
        DirSelectionDialogBase->setMinimumSize(QSize(500, 300));
        verticalLayout = new QVBoxLayout(DirSelectionDialogBase);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textLabel = new QLabel(DirSelectionDialogBase);
        textLabel->setObjectName(QString::fromUtf8("textLabel"));

        verticalLayout->addWidget(textLabel);

        treeWidget = new QTreeWidget(DirSelectionDialogBase);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setSortingEnabled(true);
        treeWidget->setHeaderHidden(true);

        verticalLayout->addWidget(treeWidget);

        labelNewFolder = new QLabel(DirSelectionDialogBase);
        labelNewFolder->setObjectName(QString::fromUtf8("labelNewFolder"));

        verticalLayout->addWidget(labelNewFolder);

        leNewFolder = new QLineEdit(DirSelectionDialogBase);
        leNewFolder->setObjectName(QString::fromUtf8("leNewFolder"));
        leNewFolder->setEnabled(true);

        verticalLayout->addWidget(leNewFolder);

        buttonBox = new QDialogButtonBox(DirSelectionDialogBase);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DirSelectionDialogBase);
        QObject::connect(buttonBox, SIGNAL(accepted()), DirSelectionDialogBase, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DirSelectionDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(DirSelectionDialogBase);
    } // setupUi

    void retranslateUi(QDialog *DirSelectionDialogBase)
    {
        DirSelectionDialogBase->setWindowTitle(QApplication::translate("DirSelectionDialogBase", "Directory Selection", 0, QApplication::UnicodeUTF8));
        textLabel->setText(QApplication::translate("DirSelectionDialogBase", "Select a folder", 0, QApplication::UnicodeUTF8));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("DirSelectionDialogBase", "1", 0, QApplication::UnicodeUTF8));
        labelNewFolder->setText(QApplication::translate("DirSelectionDialogBase", "Or create new folder:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DirSelectionDialogBase: public Ui_DirSelectionDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRSELECTIONDIALOGBASE_H
