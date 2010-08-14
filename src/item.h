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

#ifndef QAJITEM_H
#define QAJITEM_H

#include <QObject>
#include <QTreeWidget>
#include <QApplication>
#include <QUrlInfo>
#include <QProgressBar>
#include <QVBoxLayout>

#include "convert.h"

/**
@author Matthias Reif
*/
class Item : public QTreeWidgetItem, public QObject
{
// Q_OBJECT
public:
    Item(QTreeWidget *parent, const QString& id="generic");
    Item(QTreeWidgetItem *parent, const QString& id="generic");

    virtual ~Item();

    QString getStatus() const {
        return status_;
    }
    bool setStatus(const QString& newStatus);
    QString getId() const {
        return id_;
    }
    QString getHash() const {
        return hash_;
    }
    QString getFilename() const {
        return filename_;
    }
    double getSize() const {
        return size_;
    }
    bool setHiddenSave(bool hidden);
    void setFileIcon(int column);
    void setFileIcon(int column, QFileInfo& file);
    void setFileIcon(int column, QUrlInfo& url);
    QProgressBar* initProgressBar(int col);
protected:
    void initProgressBar(QProgressBar& progressBar, int col);
    static void updateProgressBar(QProgressBar& progressBar,
                const QString& from, const QString& to, const QString& current);
    static void updateProgressBar(QProgressBar& progressBar, int from, int to, int current);
    static int progressPercentualValue(const QProgressBar& progressBar);
    QString id_;
    QString status_;

    double  size_;
    QString hash_;
    QString filename_;
};

#endif
