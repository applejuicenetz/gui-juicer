/***************************************************************************
 *   Copyright (C) 2004 - 2009 by Matthias Reif, Holger Gerth              *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *   holger.gerth@informatik.tu-chemnitz.de                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see http://www.gnu.org/licenses/     *
 ***************************************************************************/

#include "modulebase.h"
#include "juicer.h"

ModuleBase::ModuleBase(Juicer* juicer, QTreeWidget* treeWidget, QToolBar* toolbar, QWidget* tabWidget) : QObject(juicer) {
    init(juicer, treeWidget, toolbar, tabWidget);
    this->treeWidget = treeWidget;
    updateAlternatingRowColors();
    connect(treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
}


ModuleBase::ModuleBase(Juicer* juicer, QWidget* contentWidget, QToolBar* toolbar, QWidget* tabWidget) : QObject(juicer) {
    init(juicer, contentWidget, toolbar, tabWidget);
}

ModuleBase::~ModuleBase() {

}

void ModuleBase::init(Juicer* juicer, QWidget* contentWidget, QToolBar* toolbar, QWidget* tabWidget) {
    this->juicer = juicer;
    this->xml = juicer->xml;
    this->contentWidget = contentWidget;
    this->toolbar = toolbar;
    this->contentWidget->insertActions(NULL, toolbar->actions());
    this->contentWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    this->tabIndex = juicer->ajTab->indexOf(tabWidget);
    this->tabText = juicer->ajTab->tabText(tabIndex);
}


/*!
    \fn ModuleBase::adjustSizeOfColumns()
 */
void ModuleBase::adjustSizeOfColumns() {
    for(int i = 0; i < treeWidget->columnCount(); i++) {
        if(!treeWidget->isColumnHidden(i)) {
            treeWidget->resizeColumnToContents(i);
        }
    }
}

/*!
    \fn ModuleBase::sortItemsInitially(const QString& settingsGroup)
 */
void ModuleBase::sortItemsInitially(const QString& settingsGroup)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup(settingsGroup);
    int column = lokalSettings.value("sortColumn", 0 ).toInt(NULL);
    int order = lokalSettings.value("sortOrder", Qt::AscendingOrder ).toInt(NULL);
    treeWidget->sortItems(column, (Qt::SortOrder)order );

    QList<QVariant> positions = lokalSettings.value("columnPositions").toList();
    for(int i=0; i<positions.size(); i++) {
        int oldIndex = treeWidget->header()->visualIndex(positions.at(i).toInt());
        treeWidget->header()->moveSection(oldIndex, i);
    }

    QList<QVariant> sizes = lokalSettings.value("columnSizes").toList();
    for(int i=0; i<sizes.size(); i++) {
        treeWidget->header()->resizeSection(i, sizes.at(i).toInt());
    }

    QList<QVariant> hidden = lokalSettings.value("columnHidden").toList();
    for(int i=0; i<hidden.size(); i++) {
        treeWidget->setColumnHidden(i, hidden.at(i).toBool());
    }
    lokalSettings.endGroup();
}

/*!
    \fn ModuleBase::saveSortOrder(const QString& settingsGroup)
 */
void ModuleBase::saveSortOrder(const QString& settingsGroup)
{
    QSettings lokalSettings;
    lokalSettings.beginGroup(settingsGroup);
    lokalSettings.setValue("sortColumn", treeWidget->sortColumn());
    lokalSettings.setValue("sortOrder", treeWidget->header()->sortIndicatorOrder());

    QList<QVariant> positions, sizes, hidden;
    for(int i=0; i<treeWidget->header()->count(); i++) {
        positions.append(treeWidget->header()->logicalIndex(i));
        sizes.append(treeWidget->header()->sectionSize(i));
        hidden.append(treeWidget->isColumnHidden(i));
    }

    lokalSettings.setValue("columnPositions", positions);
    lokalSettings.setValue("columnSizes", sizes);
    lokalSettings.setValue("columnHidden", hidden);
    lokalSettings.endGroup();
}


/*!
    \fn ModuleBase::linkListSlot()
 */
void ModuleBase::linkListSlot() {
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
                QItemList selectedItems = treeWidget->selectedItems();
                for(int i=0; i<selectedItems.size() && !error; i++) {
                    Item* item = (Item*)selectedItems[i];
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
            QString error = Convert::getFileErrorString(ajListFile.error());
            QMessageBox::critical(juicer, tr("Error while saving link list."), tr("The error message was:\n\n") + error);
        }
    }
}


/*!
    \fn ModuleBase::updateAlternatingRowColors()
 */
void ModuleBase::updateAlternatingRowColors() {
    this->treeWidget->setAlternatingRowColors(OptionsDialog::getSetting("altRows", false).toBool());
}


/*!
    \fn ModuleBase::process(QItemList& items, XMLModule::Type type, const QString& request, const QString& para)
 */
void ModuleBase::process(QItemList& items, XMLModule::Type type, const QString& request, const QString& para) {
    for(QItemList::iterator i = items.begin(); i!=items.end(); i++) {
        xml->make(type, request, para + "&id=" + ((Item*)(*i))->getId());
    }
}

/*!
    \fn ModuleBase::processSelected(XMLModule::Type type, const QString& request, const QString& para)
 */
void ModuleBase::processSelected(XMLModule::Type type, const QString& request, const QString& para) {
    QItemList items = treeWidget->selectedItems();
    process(items, type, request, para);
    juicer->timerSlot();
}

/*!
    \fn ModuleBase::processIdX(QItemList& items, const QString& request, const QString& para)
 */
void ModuleBase::processIdX(QItemList& items, const QString& request, const QString& para) {
    // -- there seems to be a bug in the appleJuice core ... --
    process(items, XMLModule::SET, request, para);
/*    if(items.empty()) {
        return;
    }
    QString ids = "&id="+((Item*)items.at(0))->getId();
    for(int i=1; i<items.size(); i++) {
        ids += "&id"+QString::number(i)+"="+((Item*)items.at(i))->getId();
    }
    qDebug() << ids;
    xml->set(request, para + ids);*/
    juicer->timerSlot();
}


/*!
    \fn ModuleBase::processSelectedIdX(const QString& request, const QString& para)
 */
void ModuleBase::processSelectedIdX(const QString& request, const QString& para) {
    QItemList items = treeWidget->selectedItems();
    processIdX(items, request, para);
}
