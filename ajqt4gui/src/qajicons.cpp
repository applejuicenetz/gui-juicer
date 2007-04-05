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
#include "qajicons.h"

#include "./xpm/ajqtgui.xpm"

#include "./xpm/close.xpm"
#include "./xpm/exit.xpm"
#include "./xpm/options.xpm"
#include "./xpm/info.xpm"

#include "./xpm/close_small.xpm"
#include "./xpm/exit_small.xpm"
#include "./xpm/options_small.xpm"
#include "./xpm/info_small.xpm"

#include "./xpm/clipboard.xpm"

#include "./xpm/increase.xpm"
#include "./xpm/decrease.xpm"

#include "./xpm/powerMax.xpm"

#include "./xpm/ok.xpm"

#include "./xpm/resume.xpm"
#include "./xpm/pause.xpm"
#include "./xpm/cancel.xpm"
#include "./xpm/filter.xpm"

#include "./xpm/resume_small.xpm"
#include "./xpm/pause_small.xpm"
#include "./xpm/cancel_small.xpm"

#include "./xpm/filter_small.xpm"

#include "./xpm/searching_small.xpm"
#include "./xpm/finished_small.xpm"

#include "./xpm/connect_small.xpm"
#include "./xpm/connected_small.xpm"

#include "./xpm/download_small.xpm"
#include "./xpm/upload_small.xpm"
#include "./xpm/share_small.xpm"

#include "./xpm/connect.xpm"

#include "./xpm/insert.xpm"
#include "./xpm/remove.xpm"
#include "./xpm/insert_small.xpm"

#include "./xpm/new.xpm"

#include "./xpm/shared_files.xpm"
#include "./xpm/reload.xpm"
#include "./xpm/reload_small.xpm"

#include "./xpm/windows.xpm"
#include "./xpm/linux.xpm"

#include "./xpm/partlist.xpm"
#include "./xpm/partlist_small.xpm"

#include "./xpm/rename.xpm"
#include "./xpm/rename_small.xpm"

#include "./xpm/save.xpm"
#include "./xpm/save_small.xpm"

#include "./xpm/warning_small.xpm"
#include "./xpm/ftp_small.xpm"

#include "./xpm/dummy.xpm"


QAjIcons::QAjIcons()
{
	ajqtguiPixmap = new QPixmap( ajqtgui_xpm );

	closePixmap = new QPixmap( close_xpm );
	exitPixmap = new QPixmap( exit_xpm );
	configurePixmap = new QPixmap( options_xpm );
	infoPixmap = new QPixmap( info_xpm );
	
	closeSmallPixmap = new QPixmap( close_small_xpm );
	exitSmallPixmap = new QPixmap( exit_small_xpm );
	configureSmallPixmap = new QPixmap( options_small_xpm );
	infoSmallPixmap = new QPixmap( info_small_xpm );

	clipboardPixmap = new QPixmap( clipboard_xpm );
	
	downloadFinishedPixmap = new QPixmap( finished_small_xpm);
	downloadLoadingPixmap = new QPixmap( resume_small_xpm );
	downloadSearchingPixmap = new QPixmap( searching_small_xpm );
	downloadPausedPixmap = new QPixmap( pause_small_xpm );
	downloadCanceldPixmap = new QPixmap( cancel_small_xpm );
	
	downloadResumePixmap = new QPixmap( resume_xpm );
	downloadPausePixmap = new QPixmap( pause_xpm );
	downloadCancelPixmap = new QPixmap( cancel_xpm );
	downloadFilterPixmap = new QPixmap( filter_xpm );
	downloadResumeSmallPixmap = new QPixmap( resume_small_xpm );
	downloadPauseSmallPixmap = new QPixmap( pause_small_xpm );
	downloadCancelSmallPixmap = new QPixmap( cancel_small_xpm );
	downloadFilterSmallPixmap = new QPixmap( filter_small_xpm );

	downloadSavePixmap = new QPixmap( save_xpm );
	downloadSaveSmallPixmap = new QPixmap( save_small_xpm );
	
	serverConnectPixmap = new QPixmap( connect_xpm );
	serverRemovePixmap = new QPixmap( cancel_xpm );
	serverConnectSmallPixmap = new QPixmap( connect_small_xpm );
	serverRemoveSmallPixmap = new QPixmap( cancel_small_xpm );
	serverConnectedSmallPixmap = new QPixmap( connected_small_xpm );
	
	powerUpPixmap = new QPixmap( increase_xpm );
	powerDownPixmap = new QPixmap( decrease_xpm );
	powerOkPixmap = new QPixmap( ok_xpm );
	powerMaxPixmap = new QPixmap( power_max_xpm );
	
	downloadSmallPixmap = new QPixmap( download_small_xpm );
	uploadSmallPixmap = new QPixmap( upload_small_xpm );
	shareSmallPixmap = new QPixmap( share_small_xpm );
	
	insertPixmap = new QPixmap( insert_xpm );
	removePixmap = new QPixmap( remove_xpm );
	insertSmallPixmap = new QPixmap( insert_small_xpm );
	
	newPixmap = new QPixmap( new_xpm );
	
	sharedFilesPixmap = new QPixmap( shared_files_xpm );
	reloadPixmap = new QPixmap( reload_xpm );
	reloadSmallPixmap = new QPixmap( reload_small_xpm );
	
	windowsPixmap = new QPixmap( windows_xpm );
	linuxPixmap = new QPixmap( linux_xpm );

	partListPixmap = new QPixmap( partlist_xpm );
	partListSmallPixmap = new QPixmap( partlist_small_xpm );

	downloadRenamePixmap = new QPixmap( rename_xpm );
	downloadRenameSmallPixmap = new QPixmap( rename_small_xpm );
	
	warningSmallPixmap = new QPixmap( warning_small_xpm );
	ftpSmallPixmap = new QPixmap( ftp_small_xpm );
	
	dummyPixmap = new QPixmap( dummy_xpm );
	
	// ====================================================================00
	
	ajqtguiIcon = new QIcon( *ajqtguiPixmap );

	closeIcon = new QIcon( *closePixmap );
	exitIcon = new QIcon( *exitPixmap );
	configureIcon = new QIcon( *configurePixmap );
	infoIcon = new QIcon( *infoPixmap );
	closeSmallIcon = new QIcon( *closeSmallPixmap );
	exitSmallIcon = new QIcon( *exitSmallPixmap );
	configureSmallIcon = new QIcon( *configureSmallPixmap );
	infoSmallIcon = new QIcon( *infoSmallPixmap );


	clipboardIcon = new QIcon( *clipboardPixmap );

	downloadFinishedIcon = new QIcon( *downloadFinishedPixmap );
	downloadLoadingIcon = new QIcon( *downloadLoadingPixmap );
	downloadSearchingIcon = new QIcon( *downloadSearchingPixmap );
	downloadPausedIcon = new QIcon( *downloadPausedPixmap );
	downloadCanceldIcon = new QIcon( *downloadCanceldPixmap );
	
	downloadResumeIcon = new QIcon( *downloadResumePixmap );
	downloadPauseIcon = new QIcon( *downloadPausePixmap );
	downloadCancelIcon = new QIcon( *downloadCancelPixmap );
	downloadFilterIcon = new QIcon( *downloadFilterPixmap );
	downloadResumeSmallIcon = new QIcon( *downloadResumeSmallPixmap );
	downloadPauseSmallIcon = new QIcon( *downloadPauseSmallPixmap );
	downloadCancelSmallIcon = new QIcon( *downloadCancelSmallPixmap );
	downloadFilterSmallIcon = new QIcon( *downloadFilterSmallPixmap );

	downloadSaveIcon = new QIcon( *downloadSavePixmap );
	downloadSaveSmallIcon = new QIcon( *downloadSaveSmallPixmap );
	
	serverConnectIcon = new QIcon( *serverConnectPixmap );
	serverRemoveIcon = new QIcon( *serverRemovePixmap );
	serverConnectSmallIcon = new QIcon( *serverConnectSmallPixmap );
	serverRemoveSmallIcon = new QIcon( *serverRemoveSmallPixmap );
	serverConnectedSmallIcon = new QIcon( *serverConnectedSmallPixmap );

	powerUpIcon = new QIcon( *powerUpPixmap );
	powerDownIcon = new QIcon( *powerDownPixmap );
	powerOkIcon = new QIcon( *powerOkPixmap );
	powerMaxIcon = new QIcon( *powerMaxPixmap );
	
	downloadSmallIcon = new QIcon( *downloadSmallPixmap );
	uploadSmallIcon = new QIcon( *uploadSmallPixmap );
	shareSmallIcon = new QIcon( *shareSmallPixmap );

	insertIcon = new QIcon( *insertPixmap );
	removeIcon = new QIcon( *removePixmap );
	insertSmallIcon = new QIcon( *insertSmallPixmap );
	
	newIcon = new QIcon( *newPixmap );
	
	sharedFilesIcon = new QIcon( *sharedFilesPixmap );
	reloadIcon = new QIcon( *reloadPixmap );
	reloadSmallIcon = new QIcon( *reloadSmallPixmap );
	
	windowsIcon = new QIcon( *windowsPixmap );
	linuxIcon = new QIcon( *linuxPixmap );
	
	partListIcon = new QIcon( *partListPixmap );
	partListSmallIcon = new QIcon( *partListSmallPixmap );
	
	downloadRenameIcon = new QIcon( *downloadRenamePixmap );
	downloadRenameSmallIcon = new QIcon( *downloadRenameSmallPixmap );
	
	warningSmallIcon = new QIcon( *warningSmallPixmap );
	ftpSmallIcon = new QIcon( *ftpSmallPixmap );
	
	dummyIcon = new QIcon( *dummyPixmap );
}

QAjIcons::~QAjIcons()
{
/*	 delete ajqtguiIcon;
	
	 delete closeIcon; delete exitIcon; delete configureIcon; delete infoIcon;
	 delete closeSmallIcon; delete exitSmallIcon; delete configureSmallIcon; delete infoSmallIcon;
	
	 delete clipboardIcon;

	 delete downloadFinishedIcon; delete downloadSearchingIcon; delete downloadLoadingIcon; delete downloadPausedIcon; delete downloadQueuedIcon; delete downloadCanceldIcon;
	
	 delete downloadResumeSmallIcon; delete downloadPauseSmallIcon; delete downloadCancelSmallIcon; delete downloadFilterSmallIcon;
	 delete downloadResumeIcon; delete downloadPauseIcon; delete downloadCancelIcon; delete downloadFilterIcon;
	
	 delete serverConnectIcon; delete serverRemoveIcon;
	 delete serverConnectSmallIcon; delete serverRemoveSmallIcon; delete serverConnectedSmallIcon;

	 delete powerUpIcon; delete powerDownIcon; delete powerOkIcon; delete powerMaxIcon;
	
	 delete downloadSmallIcon; delete uploadSmallIcon; delete shareSmallIcon;
	
	 delete insertIcon; delete removeIcon; delete newIcon;
	 delete insertSmallIcon;
	
	 delete sharedFilesIcon;
	 delete reloadIcon; delete reloadSmallIcon;
	
	 delete linuxIcon; delete windowsIcon;
	
	 delete dummyIcon;
	 */
	 // ================================

	 delete ajqtguiPixmap;
 
	 delete exitPixmap; delete closePixmap; delete configurePixmap; delete infoPixmap;
	 delete exitSmallPixmap; delete closeSmallPixmap; delete configureSmallPixmap; delete infoSmallPixmap;

	
	 delete clipboardPixmap;

	 delete downloadFinishedPixmap; delete downloadSearchingPixmap; delete downloadLoadingPixmap; delete downloadPausedPixmap; delete downloadCanceldPixmap;
	
	 delete downloadResumeSmallPixmap; delete downloadPauseSmallPixmap; delete downloadCancelSmallPixmap; delete downloadFilterSmallPixmap;
	 delete downloadResumePixmap; delete downloadPausePixmap; delete downloadCancelPixmap; delete downloadFilterPixmap;
	
	 delete serverConnectPixmap; delete serverRemovePixmap;
	 delete serverConnectSmallPixmap; delete serverRemoveSmallPixmap; delete serverConnectedSmallPixmap;
	
	 delete powerUpPixmap; delete powerDownPixmap; delete powerOkPixmap; delete powerMaxPixmap;
	
	 delete downloadSmallPixmap; delete uploadSmallPixmap; delete shareSmallPixmap;
	
	 delete insertPixmap; delete removePixmap; delete newPixmap;
	 delete insertSmallPixmap;
	
	 delete sharedFilesPixmap;
	 delete reloadPixmap; delete reloadSmallPixmap;
	
	 delete linuxPixmap; delete windowsPixmap;

	 delete dummyPixmap;

}

