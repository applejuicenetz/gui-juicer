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
#ifndef QAJLOGINDIALOG_H
#define QAJLOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

/**
@author Matthias Reif
*/
class QAjLoginDialog : public QDialog
{
    Q_OBJECT
public:
    QAjLoginDialog(QString host, QString port, QString password, QString errorMsg, QWidget *parent = 0, const char *name = 0);

    ~QAjLoginDialog();
    QString getHost()
    {
        return hostEdit->text();
    }
    QString getPort()
    {
        return portEdit->text();
    }
    QString getPassword()
    {
        return passwordEdit->text();
    }

protected:
    QLabel *messageLabel;
    QLabel *hostLabel;
    QLineEdit *hostEdit;
    QLabel *portLabel;
    QLineEdit *portEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QPushButton *retryButton;
    QPushButton *cancelButton;
protected slots:
    void retryClicked();
    void cancelClicked();

};

#endif
