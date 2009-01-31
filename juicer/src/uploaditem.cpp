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
#include "uploaditem.h"
#include "juicer.h"

UploadItem::UploadItem( QString id, QString shareId, QTreeWidget *parent ) 
  : Item(parent, id)
  , shareId(shareId)
{
    status_ = NEW_UPLOAD;
    speed = 0.0;
}


UploadItem::~UploadItem()
{
}


bool UploadItem::operator<( const QTreeWidgetItem & other ) const {
    int sortIndex = treeWidget()->header()->sortIndicatorSection();
    UploadItem* upItem = (UploadItem*)&other;
    switch ( sortIndex ) {
        case SPEED_COL:
            return this->speed < upItem->speed;
        default:
            return this->text(sortIndex) < other.text(sortIndex);
    }
}

/*!
    \fn UploadItem::update(const QIcon& osIcon,const QString& status,
        const QString& statusDescr, const QString& directState,
        const QString& priority, const QString& nick, const QString& speed, bool newUpload)
 */
void UploadItem::update(const QIcon& osIcon,const QString& status,
        const QString& statusDescr, const QString& directState,
        const QString& priority, const QString& nick, const QString& speed, const QString& version, bool newUpload) {

    this->speed = speed.toDouble();
    setStatus(status);
    if(newUpload) {
        setText(NICK_COL, nick);
        setIcon(OS_COL, osIcon);
        setText(VERSION_COL, version);
    }
    setText(SPEED_COL, QConvert::bytes(speed) + "/s" );
    setText(STATUS_COL, statusDescr);
    setText(DIRECTSTATE_COL, directState);
    setText(PRIORITY_COL, priority );
}
