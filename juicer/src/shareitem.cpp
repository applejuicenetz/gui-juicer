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
#include "shareitem.h"

ShareItem::ShareItem(QTreeWidget* parent, const QString& path, bool recursive) : Item(parent) {
    this->path = path;
    this->recursive = recursive;
    setText(ShareItem::PATH_COL, path);
    if(recursive) {
        setIcon(PATH_COL, QIcon(":/small/recursive.png"));
    } else {
        setIcon(PATH_COL, QIcon(":/small/not_recursive.png"));
    }

}

ShareItem::~ShareItem() {
}


/*!
    \fn ShareItem::insertSharedFile(ShareFileItem* sharedFile)
 */
void ShareItem::insertSharedFile(ShareFileItem* sharedFile)
{
    if ( sharedFile != NULL ) sharedFiles.append(sharedFile);
}


/*!
    \fn ShareItem::update()
 */
void ShareItem::update()
{
    double sum = 0.0;
    for(int i=0; i<sharedFiles.size(); i++) {
        sum += sharedFiles.at(i)->getSize();
    }
    setText(ShareItem::SIZE_COL, QConvert::bytes(sum) + " (" + QString::number(sharedFiles.size()) + ")");
}
