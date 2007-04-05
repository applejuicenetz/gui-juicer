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
#include "qxmlhandler.h"

QXMLHandler::QXMLHandler(QXMLModule *xmlModule)
{
	xml = xmlModule;
}

QXMLHandler::~QXMLHandler()
{
}

bool QXMLHandler::startDocument()
{
	return xml->startDocument();
}

bool QXMLHandler::endDocument()
{
	return xml->endDocument();
}

bool QXMLHandler::startElement( const QString& s1, const QString& s2,
                                    const QString& qName,
                                    const QXmlAttributes& attr)
{
	return xml->startElement(s1, s2, qName, attr);
}

bool QXMLHandler::endElement( const QString& s1, const QString& s2, const QString& qName )
{
	return xml->endElement(s1, s2, qName);
}

bool QXMLHandler::characters( const QString& s1 )
{
	return xml->characters(s1);
}
