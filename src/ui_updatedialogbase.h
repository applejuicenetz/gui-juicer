/********************************************************************************
** Form generated from reading UI file 'updatedialogbase.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATEDIALOGBASE_H
#define UI_UPDATEDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QProgressBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_UpdateDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QProgressBar *progressBar;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *UpdateDialog)
    {
        if (UpdateDialog->objectName().isEmpty())
            UpdateDialog->setObjectName(QString::fromUtf8("UpdateDialog"));
        UpdateDialog->resize(365, 279);
        verticalLayout = new QVBoxLayout(UpdateDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        textEdit = new QTextEdit(UpdateDialog);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setUndoRedoEnabled(false);
        textEdit->setReadOnly(true);

        verticalLayout->addWidget(textEdit);

        progressBar = new QProgressBar(UpdateDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setEnabled(false);
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        buttonBox = new QDialogButtonBox(UpdateDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::No|QDialogButtonBox::Yes);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(UpdateDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), UpdateDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), UpdateDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(UpdateDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateDialog)
    {
        UpdateDialog->setWindowTitle(QApplication::translate("UpdateDialog", "Dialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UpdateDialog: public Ui_UpdateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATEDIALOGBASE_H
