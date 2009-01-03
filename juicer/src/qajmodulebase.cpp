//
// C++ Implementation: qajmodulebase
//
// Description: 
//
//
// Author: Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajmodulebase.h"
#include "juicer.h"
#include "qxmlmodule.h"

QAjModuleBase::QAjModuleBase(Juicer* juicer, QTreeWidget* treeWidget, QToolBar* toolbar) : QObject(juicer) {
    this->juicer = juicer;
    this->xml = juicer->xml;
    this->treeWidget = treeWidget;
    this->toolbar = toolbar;
    this->treeWidget->insertActions(NULL, toolbar->actions());
    this->treeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
}


QAjModuleBase::~QAjModuleBase() {

}

/*!
    \fn QAjModuleBase::adjustSizeOfColumns()
 */
void QAjModuleBase::adjustSizeOfColumns() {
    for(int i = 0; i < treeWidget->columnCount(); i++) {
        treeWidget->resizeColumnToContents(i);
    }
}

/*!
    \fn QAjModuleBase::sortItemsInitially(QString settingsGroup)
 */
void QAjModuleBase::sortItemsInitially(QString settingsGroup)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup(settingsGroup);
    int column = lokalSettings.value("sortColumn", 0 ).toInt(NULL);
    int order = lokalSettings.value("sortOrder", Qt::AscendingOrder ).toInt(NULL);
    treeWidget->sortItems(column, (Qt::SortOrder)order );
    lokalSettings.endGroup();
}

/*!
    \fn QAjModuleBase::saveSortOrder(QString settingsGroup)
 */
void QAjModuleBase::saveSortOrder(QString settingsGroup)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup(settingsGroup);
    lokalSettings.setValue("sortColumn", treeWidget->sortColumn());
    lokalSettings.setValue("sortOrder", treeWidget->header()->sortIndicatorOrder());
    lokalSettings.endGroup();
}


/*!
    \fn QAjModuleBase::linkListSlot()
 */
void QAjModuleBase::linkListSlot() {
    QFileDialog fileDialog(juicer, tr("Enter file name"), QString::null, tr("AJ Link Lists (*.ajl)"));
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setConfirmOverwrite(true);
    fileDialog.setDefaultSuffix("ajl");
    if(fileDialog.exec()) {
        QFile ajListFile(fileDialog.selectedFiles().takeFirst());
        if(ajListFile.exists()) {
            ajListFile.remove();
        }
        bool error = !ajListFile.open(QIODevice::WriteOnly | QIODevice::Text);
        if(!error) {
            QString message = "appleJuice link list\nCreated by Juicer, the appleJuice GUI based on Qt4.\n\n";
            message += "The developers of Juicer take no responsibility for the files listed below!\n100\n";
            if(ajListFile.write( message.toAscii()) == -1) {
                error = true;
            } else {
                ajListFile.setPermissions(
                        QFile::ReadOwner | QFile::WriteOwner |
                        QFile::ReadUser | QFile::WriteUser |
                        QFile::ReadGroup | QFile::ReadOther);
                QList<QTreeWidgetItem *> selectedItems = treeWidget->selectedItems();
                for(int i=0; i<selectedItems.size() && !error; i++) {
                    QAjItem* item = (QAjItem*)selectedItems[i];
                    error =
                           ajListFile.write(QString(item->getFilename() + '\n').toAscii()) == -1
                        || ajListFile.write(QString(item->getHash() + '\n').toAscii()) == -1
                        || ajListFile.write(QString(QString::number((int)item->getSize()) + '\n').toAscii()) == -1;
                }
            }
            error = error || !ajListFile.flush();
            ajListFile.close();
        }
        if(error) {
            QString error = QConvert::getFileErrorString(ajListFile.error());
            QMessageBox::critical(juicer, tr("Error while saving link list."), tr("The error message was:\n\n") + error);
        }
    }
}
