/***************************************************************************
 *   Copyright (C) 2007 by Matthias Reif   *
 *   matthias.reif@iupr.dfki.de   *
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

#ifndef QAJNETWORKDIALOG_H
#define QAJNETWORKDIALOG_H

#include <QDialog>
#include "ui_networkdialogbase.h"

class NetworkDialog : public QDialog, private Ui::NetworkDialog
{
  Q_OBJECT

public:
  NetworkDialog(QWidget* parent = 0, Qt::WFlags fl = 0 );
  ~NetworkDialog();
    void setValues( const QString& users, const QString& files, const QString& size, const QString& ip, const QString& firewalled );


};

#endif

