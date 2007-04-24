//
// C++ Implementation: qajfiledialog
//
// Description: 
//
//
// Author: Holger Gerth <holger.gerth@informatik.tu-chemnitz.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qajfiledialog.h"

QAjFileDialog::QAjFileDialog( QXMLModule *xml, QWidget *parent )
 : QDialog( parent )
{
    this->setFixedSize(400,650);

    fileSystem = new QAjFileWidget( xml, this );
    fileSystem->setFixedSize(400,600);
    fileSystem->setColumnWidth( 0, 396 );

//     this->xml = xml;

    xml->get( "directory" );
}


QAjFileDialog::~QAjFileDialog()
{
}

void QAjFileDialog::insertDirectory( QString dir, int type ) {
    if ( fileSystem != NULL ) fileSystem->insertDirectory( dir, type );
}

void QAjFileDialog::insertSeperator( QString seperator ) {
    if ( fileSystem != NULL ) fileSystem->insertSeperator( seperator );
}


