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

    okButton = new QDialogButtonBox(QDialogButtonBox::Ok);
    okButton->setParent( this );
    okButton->setGeometry ( 50, 600, 100, 50 );

    cancelButton = new QDialogButtonBox(QDialogButtonBox::Cancel);
    cancelButton->setParent( this );
    cancelButton->setGeometry ( 200, 600, 100, 50 );

    connect ( okButton, SIGNAL (clicked ( QAbstractButton * )), this, SLOT (getPathSlot()));
    connect ( cancelButton, SIGNAL (clicked ( QAbstractButton * )), this, SLOT (close()));

    xml->get( "directory" );
}


QAjFileDialog::~QAjFileDialog()
{
}

void QAjFileDialog::insertDirectory( QString dir, QString path, int type ) {
    if ( fileSystem != NULL ) fileSystem->insertDirectory( dir, path, type );
}

void QAjFileDialog::insertSeperator( QString seperator ) {
    if ( fileSystem != NULL ) fileSystem->insertSeperator( seperator );
    this->filesystemSeperator = seperator;
}

QString QAjFileDialog::getSeperator() {
    return filesystemSeperator;
}

void QAjFileDialog::getPathSlot() {

    newSharePath = fileSystem->getPathFromSelectedItem();

    this->close();
}

QString QAjFileDialog::getDirectory() {
    return newSharePath;
}

