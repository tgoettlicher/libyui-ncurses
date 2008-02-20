/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|						     (c) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:		NCPackageSelectorPluginStub.cc

  Author:	Hedgehog Painter <kmachalkova@suse.cz>	


/-*/

#include "NCPackageSelectorPluginStub.h"

#define YUILogComponent "ncurses-ui"
#include "YUILog.h"
#include "NCWidget.h"
#include "NCLabel.h"
#include "NCDialog.h"
#include "NCPackageSelectorPluginIf.h"
//#include "NCPackageSelectorStart.h"
//#include "NCPkgTable.h"
#include "YNCursesUI.h" // NCtoY2Event

#define PLUGIN_BASE_NAME "ncurses_pkg"

NCPackageSelectorPluginStub::NCPackageSelectorPluginStub()
    : YPackageSelectorPlugin( PLUGIN_BASE_NAME )
{
    if ( success() )
    {
	yuiMilestone() << "Loaded " << PLUGIN_BASE_NAME
                       << " plugin successfully from " << pluginLibFullPath()
                       << endl;
    }


    impl =  (NCPackageSelectorPluginIf*) locateSymbol("PSP");  
    if (!impl)
	throw NCursesError( "Cannot load Package Selector Plugin" );
}


NCPackageSelectorPluginStub::~NCPackageSelectorPluginStub()
{
    // NOP
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPackageSelectorPluginStub::createPackageSelector
//	METHOD TYPE : YWidget
//
//	DESCRIPTION : Create NCPackageSelectorStart which reads the layout
//                    term of the package selection dialog, creates the widget
//		      tree and creates the NCPackageSelector.
//
YPackageSelector * NCPackageSelectorPluginStub::createPackageSelector( YWidget * parent,
								   long modeFlags )
{
    return impl->createPackageSelector( parent, modeFlags );    
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPackageSelectorPluign::createPkgSpecial
//	METHOD TYPE : YWidget
//
//	DESCRIPTION : creates special widgets used for the package selection
//		      dialog (which do not have a corresponding widget in qt-ui)
//
YWidget * NCPackageSelectorPluginStub::createPkgSpecial( YWidget *parent, const string &subwidget )
{
    return impl->createPkgSpecial( parent, subwidget );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPackageSelectorPLugin::runPkgSelection
//	METHOD TYPE : YEvent *
//
//	DESCRIPTION : Implementation of UI builtin RunPkgSelection() which
//		      has to be called after OpenDialog( `PackageSelector() ).
//
YEvent * NCPackageSelectorPluginStub::runPkgSelection(  YDialog * dialog,
						    YWidget * selector )
{
    return impl->runPkgSelection( dialog, selector );    
}

