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

#include "item.h"

Item::Item( QTreeWidget *parent, const QString& id ) : QTreeWidgetItem( parent ), QObject()
  , id_(id)
  , status_("-1")
  , size_(0.0)
{
}

Item::Item( QTreeWidgetItem *parent, const QString& id ) : QTreeWidgetItem( parent ), QObject()
  , id_(id)
  , status_("-1")
  , size_(0.0)
{
}


Item::~Item() {
    setHiddenSave(false);
}


/*!
    \fn Item::setHiddenSave(bool hidden)
 */
bool Item::setHiddenSave(bool hidden) {
    bool r = isHidden();
    if(r != hidden) {
        qApp->processEvents();
        setHidden(hidden);
        qApp->processEvents();
    }
    return r;
}

void Item::setFileIcon(int column) {
    setIcon(column, Convert::getFileIcon(text(column)));
}

void Item::setFileIcon(int column, QFileInfo& file) {
    setIcon(column, Convert::getFileIcon(file));
}

void Item::setFileIcon(int column, QUrlInfo& url) {
    setIcon(column, Convert::getFileIcon(url));
}

bool Item::setStatus(const QString& status) {
    bool newStatus = status_ != status;
    if(newStatus) {
        status_ = status;
    }
    return newStatus;
}


void Item::initProgressBar(QProgressBar& progressBar, int col) {
    progressBar.setMaximumHeight(18);
    QWidget* progressBarWidget = new QWidget(treeWidget());
    progressBarWidget->setContentsMargins(5, 0, 5, 0);
    QVBoxLayout* l = new QVBoxLayout();
    l->setSpacing(0);
    l->setMargin(1);
    l->addWidget(&progressBar);
    progressBarWidget->setLayout(l);
    treeWidget()->setItemWidget(this, col, progressBarWidget);
}

QProgressBar* Item::initProgressBar(int col) {
    QWidget* progressBarWidget = new QWidget(treeWidget());
    QProgressBar* progressBar = new QProgressBar(progressBarWidget);
    progressBar->setMaximumHeight(18);
    progressBarWidget->setContentsMargins(5, 0, 5, 0);
    QVBoxLayout* l = new QVBoxLayout();
    l->setSpacing(0);
    l->setMargin(1);
    l->addWidget(progressBar);
    progressBarWidget->setLayout(l);
    treeWidget()->setItemWidget(this, col, progressBarWidget);
    return progressBar;
}


/*!
    \fn Item::updateProgressBar(QProgressBar& progressBar, const QString& from, const QString& to, const QString& current)
 */
void Item::updateProgressBar(QProgressBar& progressBar,
                const QString& from, const QString& to, const QString& current) {
    updateProgressBar(progressBar, from.toInt(), to.toInt(), current.toInt());
}


/*!
    \fn Item::updateProgressBar(QProgressBar& progressBar, int from, int to, int current)
 */
void Item::updateProgressBar(QProgressBar& progressBar, int from, int to, int current) {
    if(from != progressBar.minimum()) {
        progressBar.setMinimum(from);
    }
    if(to != progressBar.maximum()) {
        progressBar.setMaximum(to);
    }
    progressBar.setValue(current);
}

int Item::progressPercentualValue(const QProgressBar& progressBar) {
    int min = progressBar.minimum();
    int max = progressBar.maximum();
    int value = progressBar.value();
    if((min == max) || (value < min)) {
        return 0;
    } else {
        return ((value-min) *100)/(max-min);
    }
}

