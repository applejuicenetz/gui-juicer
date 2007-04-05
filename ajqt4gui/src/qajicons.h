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
#ifndef QAJICONS_H
#define QAJICONS_H

#include <QPixmap>
#include <QIcon>


/**
@author Matthias Reif
*/
class QAjIcons{
public:
	QAjIcons();
	~QAjIcons();
	
	QPixmap *ajqtguiPixmap;
 
	QPixmap *exitPixmap, *closePixmap, *configurePixmap, *infoPixmap;
	QPixmap *exitSmallPixmap, *closeSmallPixmap, *configureSmallPixmap, *infoSmallPixmap;

	
	QPixmap *clipboardPixmap;

	QPixmap *downloadFinishedPixmap, *downloadSearchingPixmap, *downloadLoadingPixmap, *downloadPausedPixmap, *downloadCanceldPixmap;
	
	QPixmap *downloadResumeSmallPixmap, *downloadPauseSmallPixmap, *downloadCancelSmallPixmap, *downloadFilterSmallPixmap;
	QPixmap *downloadResumePixmap, *downloadPausePixmap, *downloadCancelPixmap, *downloadFilterPixmap;

	QPixmap *downloadSavePixmap, *downloadSaveSmallPixmap;
	
	QPixmap *serverConnectPixmap, *serverRemovePixmap;
	QPixmap *serverConnectSmallPixmap, *serverRemoveSmallPixmap, *serverConnectedSmallPixmap;
	
	QPixmap *powerUpPixmap, *powerDownPixmap, *powerOkPixmap, *powerMaxPixmap;
	
	QPixmap *downloadSmallPixmap, *uploadSmallPixmap, *shareSmallPixmap;
	
	QPixmap *insertPixmap, *removePixmap, *newPixmap;
	QPixmap *insertSmallPixmap;
	
	QPixmap *sharedFilesPixmap;
	QPixmap *reloadPixmap, *reloadSmallPixmap;
	
	QPixmap *linuxPixmap, *windowsPixmap;
	
	QPixmap *partListPixmap, *partListSmallPixmap;
	QPixmap *downloadRenamePixmap, *downloadRenameSmallPixmap;
	
	QPixmap *warningSmallPixmap;
	QPixmap *ftpSmallPixmap;

	QPixmap *dummyPixmap;
	
	// ================================================================================
	
	QIcon *ajqtguiIcon;
	
	QIcon *closeIcon, *exitIcon, *configureIcon, *infoIcon;
	QIcon *closeSmallIcon, *exitSmallIcon, *configureSmallIcon, *infoSmallIcon;
	
	QIcon *clipboardIcon;

	QIcon *downloadFinishedIcon, *downloadSearchingIcon, *downloadLoadingIcon, *downloadPausedIcon, *downloadQueuedIcon, *downloadCanceldIcon;
	
	QIcon *downloadResumeSmallIcon, *downloadPauseSmallIcon, *downloadCancelSmallIcon, *downloadFilterSmallIcon;
	QIcon *downloadResumeIcon, *downloadPauseIcon, *downloadCancelIcon, *downloadFilterIcon;

	QIcon *downloadSaveIcon, *downloadSaveSmallIcon;
	
	QIcon *serverConnectIcon, *serverRemoveIcon;
	QIcon *serverConnectSmallIcon, *serverRemoveSmallIcon, *serverConnectedSmallIcon;

	QIcon *powerUpIcon, *powerDownIcon, *powerOkIcon, *powerMaxIcon;
	
	QIcon *downloadSmallIcon, *uploadSmallIcon, *shareSmallIcon;
	
	QIcon *insertIcon, *removeIcon, *newIcon;
	QIcon *insertSmallIcon;
	
	QIcon *sharedFilesIcon;
	QIcon *reloadIcon, *reloadSmallIcon;
	
	QIcon *linuxIcon, *windowsIcon;
	
	QIcon *partListIcon, *partListSmallIcon;
	
	QIcon *downloadRenameIcon, *downloadRenameSmallIcon;
	
	QIcon *warningSmallIcon;
	QIcon *ftpSmallIcon;
	
	QIcon *dummyIcon;
};

#endif
