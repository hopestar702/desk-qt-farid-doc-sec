/***************************************************************************
 *   Copyright (C) 2007-2008 by Francesco Cecconi                          *
 *   francesco.cecconi@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
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

#include "../mainwin.h"

void nmapClass::setNmapsiSlot() {

	connect( portCombo, SIGNAL(activated(const QString&)), 
		 this, SLOT(update_portCombo()) ); // portCombo slot
	connect( comboScan, SIGNAL(activated(const QString&)), 
		 this, SLOT(update_scanCombo()) ); // comboScan slot

	connect( checkInputFile, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_inputcheck()) ); // input check
	connect( actionStop_Scan, SIGNAL( triggered() ), 
		 this, SLOT( stop_scan() ) ); // stop scan button slot

	// discover
	connect( checkTcpPing, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_discover()) ); // discover check
	connect( checkTcpSyn, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_discover()) ); // discover check
	connect( checkUdpPing, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_discover()) ); // discover check

	// action slot
	connect( action_Quit, SIGNAL( triggered() ), 
		 this, SLOT( exit() ) ); // file action menu
	connect( action_Quit_2, SIGNAL( triggered() ), 
		 this, SLOT( exit() ) ); // file action menu
	connect( action_About, SIGNAL( triggered() ), 
		 this, SLOT( about() ) ); // about action menu
	connect( actionAbout_Qt, SIGNAL( triggered() ), 
		 this, SLOT( about_qt() ) ); // about action menu
	connect( action_Scan_menu, SIGNAL( triggered() ), 
		 this, SLOT( scan() ) ); // about action menu
	connect( actionClear_History, SIGNAL( triggered() ), 
		 this, SLOT( listClear() ) ); 
	connect( action_Scan_2, SIGNAL( triggered() ), 
		 this, SLOT( scan() ) ); // about action menu
	connect( buttonBrowser, SIGNAL( clicked() ), 
		 this, SLOT( input_browser() ) ); // Input file Browser slot

	// Timing Slot (Enable and Disable spinBox)
	connect( TcheckIpv4ttl, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );
	connect( TcheckMinPar, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );
	connect( TcheckMaxPar, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );
	connect( TcheckInitRtt, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );
	connect( TcheckMinRtt, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );
	connect( TcheckMaxRtt, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );
	connect( TcheckHostTime, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );
	connect( TcheckScanDelay, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_timing()) );

	//Options
	connect( checkBoxDevice, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_options()));
	connect( checkDecoy, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_options()));
	connect( checkSpoof, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_options()));
	connect( checkSourcePort, SIGNAL( toggled(bool) ), 
		 this, SLOT(update_options()));
	connect( comboVerbosity, SIGNAL( activated(const QString&) ), 
		 this, SLOT(update_comboVerbosity()));

	// resize tab
 	connect( bscanWid, SIGNAL( clicked() ), 
		 this, SLOT(resize_tab_Options_Scan()));
	connect( boption, SIGNAL( clicked() ), 
		 this, SLOT(resize_tab_Options()));
	connect( bmiscOptions, SIGNAL( clicked() ), 
		 this, SLOT(resize_tab_MiscOptions()));
	connect( bdiscover, SIGNAL( clicked() ), 
		 this, SLOT(resize_tab_Discover()));
	connect( bfiles, SIGNAL( clicked() ), 
		 this, SLOT(resize_tab_Files()));
	connect( btiming, SIGNAL( clicked() ), 
		 this, SLOT(resize_tab_Timing()));

	connect( actionProfile, SIGNAL( triggered() ), 
		 this, SLOT(startProfile_ui()));
	connect( action_Save_As, SIGNAL( triggered() ), 
		 this, SLOT(saveAsLog()));
	connect( actionSave, SIGNAL( triggered() ), 
		 this, SLOT(saveLog()));
	connect( actionSave_As_Menu, SIGNAL( triggered() ), 
		 this, SLOT(saveAsLog()));
	connect( actionSave_Menu, SIGNAL( triggered() ), 
		 this, SLOT(saveLog()));
	connect( actionClose_Tree, SIGNAL( triggered() ), 
		 this, SLOT(closeTree()));
	connect( actionFullS, SIGNAL( triggered() ), 
		 this, SLOT(checkFullScreen()));
	connect( actionMenuBar, SIGNAL( triggered() ), 
		 this, SLOT(updateMenuBar()));
	connect( actionMain_Toolbars, SIGNAL( triggered() ), 
		 this, SLOT(showMainToolBar()));
	connect( actionActions_Toolbar, SIGNAL( triggered() ), 
		 this, SLOT(showActionToolBar()));
	connect( actionShow_Status_Bar, SIGNAL( triggered() ), 
		 this, SLOT(showStatusBar()));

	connect( hostEdit, SIGNAL( returnPressed() ),
		 this, SLOT(scan()));

	connect( toolBox, SIGNAL(currentChanged(int)),
		 this, SLOT(updateIconsBox()));

	connect( treeLogH, SIGNAL(itemSelectionChanged()), 
		 this, SLOT(callScanH()));

	// Vuln signal
	connect( buttonVuln, SIGNAL(clicked()), 
		 this, SLOT(searchVuln()));
	connect( viewVuln, SIGNAL(loadProgress(int)), 
		 progressScan, SLOT(setValue(int)));
	connect( Bleft, SIGNAL(clicked()), 
		 viewVuln, SLOT(back()));
	connect( Bright, SIGNAL(clicked()), 
		 viewVuln, SLOT(forward()));
	connect( Bstop, SIGNAL(clicked()), 
		 viewVuln, SLOT(stop()));

}
