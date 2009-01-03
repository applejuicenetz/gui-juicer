/***************************************************************************
 *   Copyright (C) 2004 by Matthias Reif                                   *
 *   matthias.reif@informatik.tu-chemnitz.de                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "qajuploadmodule.h"
#include "juicer.h"

QAjUploadModule::QAjUploadModule(Juicer* juicer) : QAjModuleBase(juicer, juicer->uploadsTreeWidget, juicer->uploadToolBar) {
    uploadStatusDescr["1"] = "active";
    uploadStatusDescr["2"] = "queueing";

    uploadDirectStateDescr["0"] = "disconnected";
    uploadDirectStateDescr["1"] = "active";
    uploadDirectStateDescr["2"] = "passive";

    connect(juicer->actionHide_Queued, SIGNAL(triggered(bool)), this, SLOT(hideQueuedSlot(bool)));
}


QAjUploadModule::~QAjUploadModule()
{}

bool QAjUploadModule::insertUpload(QString id, QString shareId, QString version, QString os, QString status, QString directState, QString priority, QString nick, QString speed) {
    QAjUploadItem *uploadItem = findUpload(id);
    if(uploadItem == NULL) {
        uploadItem = new QAjUploadItem(id, shareId, treeWidget);
        uploads[ id ] = uploadItem;
        uploadItem->setText(NICK_UP_INDEX, nick);
        uploadItem->setText(FILENAME_UP_INDEX, "");
        uploadItem->setIcon(OS_UP_INDEX, juicer->osIcons[os]);
    }

    uploadItem->setStatus(status);
    uploadItem->setText(SPEED_UP_INDEX, QConvert::bytes(speed) + "/s" );
    uploadItem->setText(STATUS_UP_INDEX, uploadStatusDescr[status]);
    uploadItem->setText(DIRECTSTATE_UP_INDEX, uploadDirectStateDescr[directState]);
    uploadItem->setText(PRIORITY_UP_INDEX, priority );

    return ( uploadItem->text( FILENAME_UP_INDEX ) != "" );
}

bool QAjUploadModule::remove(QString id) {
    QTreeWidgetItem *item = findUpload(id);
    if(item != NULL) {
        uploads.remove(id);
        delete item;
        return true;
    }
    return false;
}


QAjUploadItem* QAjUploadModule::findUpload(QString id) {
    if(uploads.contains(id)) {
        return uploads[id];
    }
    return NULL;
}


/*!
    \fn QAjUploadModule::setFilename(QString shareId, QString filename)
 */
void QAjUploadModule::setFilename(QString shareId, QString filename) {
    QHash<QString, QAjUploadItem*>::const_iterator item;
    for(item = uploads.constBegin(); item != uploads.constEnd(); item++) {
        if((*item)->shareId == shareId) {
            (*item)->setText(FILENAME_UP_INDEX, filename);
        }
    }
}


/*!
    \fn QAjUploadModule::adjustSizeOfColumns()
 */
void QAjUploadModule::adjustSizeOfColumns() {
    for(int i = 0; i < treeWidget->columnCount(); i++) {
        treeWidget->resizeColumnToContents(i);
    }
}

void QAjUploadModule::selectionChanged() {

}

/*!
    \fn QAjUploadModule::hideQueuedSlot(bool checked)
 */
void QAjUploadModule::hideQueuedSlot(bool checked) {
    QHash<QString,QAjUploadItem*>::const_iterator i;
    for(i = uploads.constBegin(); i != uploads.constEnd(); i++) {
        (*i)->setHidden(checked && ((*i)->getStatus() == QUEUEING_UPLOAD));
    }
}
