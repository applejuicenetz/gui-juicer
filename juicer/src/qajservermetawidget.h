/***************************************************************************
 *   Copyright (C) 2007 by Matthias Reif   *
 *   matthias.reif@informatik.tu-chemnitz.de   *
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
#ifndef QAJSERVERMETAWIDGET_H
#define QAJSERVERMETAWIDGET_H

#include <QtGlobal>
#include <QMainWindow>
#include <QWidget>
#include <QDockWidget>
#include <QSettings>
#include <QTextEdit>

#include "qajserverwidget.h"

/**
	@author Matthias Reif <matthias.reif@informatik.tu-chemnitz.de>
*/
class QAjServerMetaWidget : public QMainWindow
{
Q_OBJECT
public:
    QAjServerMetaWidget(QWidget *parent = 0);

    ~QAjServerMetaWidget();
    void setServerWidget(QAjServerWidget* serverWidget);

    QAjServerWidget* serverWidget;
    QTextEdit *welcomeMessage;
    QAction* showWelcomeAction;

    class AjDockWidget : public QDockWidget
    {
    public:
        AjDockWidget(QString title, QAjServerMetaWidget* parent)
            : QDockWidget(title, parent) {enabled = false;}
        bool enabled;

        QSize sizeHint() const {
            QSettings localSettings;
            localSettings.beginGroup("WelcomeDock");
            return localSettings.value("size", true).toSize();
        }
    protected:
        void closeEvent( QCloseEvent * event )
        {
            enabled = false;
            ((QAjServerMetaWidget*)this->parentWidget())->showWelcomeAction->setChecked(false);
            QWidget::closeEvent( event );
        }
    } *dock;


private slots:
    void welcomeDockLocationChanged( Qt::DockWidgetArea area );
    void dockVisibleSlot( bool visible );
};

#endif
