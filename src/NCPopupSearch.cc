/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                        (C) SuSE GmbH |
\----------------------------------------------------------------------/

   File:       NCPopupSearch.cc

   Author:     Gabriele Strattner <gs@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#include "Y2Log.h"
#include "NCPopupSearch.h"

#include "NCTree.h"
#include "YMenuButton.h"
#include "YDialog.h"
#include "NCSplit.h"
#include "NCSpacing.h"
#include "NCFrame.h"

#include "PkgNames.h"
#include "PackageSelector.h"

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupSearch::NCPopupSearch
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
NCPopupSearch::NCPopupSearch( const wpos at, PackageSelector * pkger )
    : NCPopup( at, false )
      , searchExpr( 0 )
      , ignoreCase( 0 )
      , checkName( 0 )
      , checkDescr( 0 )
      , checkProvides( 0 )
      , checkRequires( 0 )
      , cancelButton( 0 )
      , packager( pkger )
{
    createLayout( YCPString(PkgNames::PackageSearch().str()) );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupSearch::~NCPopupSearch
//	METHOD TYPE : Destructor
//
//	DESCRIPTION :
//
NCPopupSearch::~NCPopupSearch()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupSearch::createLayout
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
void NCPopupSearch::createLayout( const YCPString & headline )
{

  YWidgetOpt opt;
 
  // vertical split is the (only) child of the dialog
  NCSplit * vSplit = new NCSplit( this, opt, YD_VERT );
  addChild( vSplit );

  NCSpacing * vSp = new NCSpacing( vSplit, opt, 0.8, false, true );
  NCSpacing * vSp1 = new NCSpacing( vSplit, opt, 0.6, false, true );

  vSplit->addChild( vSp );
  
  // add the headline
  opt.isHeading.setValue( true );
  NCLabel * head = new NCLabel( vSplit, opt, headline );
  vSplit->addChild( head );

  vSplit->addChild( vSp1 );

  // add the input field (a editable combo box) 
  opt.isHStretchable.setValue( true );

  NCFrame * frame0 = new NCFrame( vSplit, opt, YCPString("" ) );
  NCSplit * vSplit2 = new NCSplit( frame0, opt, YD_VERT );
  
  opt.isEditable.setValue( true );
  searchExpr = new NCComboBox( frame0, opt, YCPString(PkgNames::SearchPhrase().str()) );
  searchExpr->setKeyboardFocus();
  frame0->addChild( searchExpr );
 
  vSplit2->addChild( vSp1 );
  vSplit->addChild( frame0 );
  
  // add the checkBox ignore case
  NCSplit * hSplit2 = new NCSplit( vSplit, opt, YD_HORIZ );
  vSplit->addChild( hSplit2 );
  NCSpacing * hSp1 = new NCSpacing( hSplit2, opt, 0.1, true, false );
  ignoreCase = new NCCheckBox( hSplit2, opt, YCPString(PkgNames::IgnoreCase().str()), true );
  hSplit2->addChild( hSp1 );
  hSplit2->addChild( ignoreCase );
  
  vSplit->addChild( vSp1 );	// VSpacing

  // add a frame contianing the check boxes
  opt.isHStretchable.setValue( true );
  opt.isVStretchable.setValue( true );
  NCFrame * frame = new NCFrame( vSplit, opt, YCPString("Search in" ) );
  NCSplit * vSplit3 = new NCSplit( frame, opt, YD_VERT );

  opt.isVStretchable.setValue( false );
  checkName = new NCCheckBox( vSplit3, opt, YCPString( "&Name and Summary" ), true );
  checkDescr = new NCCheckBox( vSplit3, opt, YCPString(PkgNames::CheckDescr().str()), false );
  checkProvides = new NCCheckBox( vSplit3, opt, YCPString( "&Provides" ), false );
  checkRequires = new NCCheckBox( vSplit3, opt, YCPString( "&Requires" ), false );
  
  vSplit3->addChild( checkName );
  vSplit3->addChild( checkDescr );
  vSplit3->addChild( checkProvides );
  vSplit3->addChild( checkRequires ); 
  
  frame->addChild( vSplit3 );
  vSplit->addChild( frame );

  vSplit->addChild( vSp1 );		// VSpacing
  
  NCSplit * hSplit3 = new NCSplit( vSplit, opt, YD_HORIZ );
  vSplit->addChild( hSplit3 );

  // add the cancel and the ok button 
  opt.isHStretchable.setValue( false );
  cancelButton = new NCPushButton( hSplit3, opt, YCPString(PkgNames::CancelLabel().str()) );
  cancelButton->setId( PkgNames::Cancel () );

  okButton = new NCPushButton( hSplit3, opt, YCPString(PkgNames::OKLabel().str()) );
  okButton->setId( PkgNames::OkButton () );

  opt.isHStretchable.setValue( true );
  NCSpacing * sp4 = new NCSpacing( hSplit3, opt, 0.2, true, false );
  NCSpacing * sp5 = new NCSpacing( hSplit3, opt, 0.4, true, false );
  hSplit3->addChild( sp4 );
  hSplit3->addChild( cancelButton );
  hSplit3->addChild( sp5 );
  hSplit3->addChild( okButton );
  hSplit3->addChild( sp4 );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupSearch::showSearchPopup
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
NCursesEvent & NCPopupSearch::showSearchPopup( )
{
    postevent = NCursesEvent();
    do {
	popupDialog();
    } while ( postAgain() );
    
    popdownDialog();

    return postevent;
}

///////////////////////////////////////////////////////////////////
//
//	DESCRIPTION :
//
YCPString  NCPopupSearch::getSearchExpression() const
{
    return searchExpr->getValue();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupSearch::niceSize
//	METHOD TYPE : void
//
//	DESCRIPTION :
//

long NCPopupSearch::nicesize(YUIDimension dim)
{
    return ( dim == YD_HORIZ ? 45 : 18);
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopup::wHandleInput
//	METHOD TYPE : NCursesEvent
//
//	DESCRIPTION :
//
NCursesEvent NCPopupSearch::wHandleInput( int ch )
{
    if ( ch == 27 ) // ESC
	return NCursesEvent::cancel;

    // if ( ch == KEY_RETURN )
    // return NCursesEvent::button;
    
    return NCDialog::wHandleInput( ch );
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : NCPopupSearch::postAgain
//	METHOD TYPE : bool
//
//	DESCRIPTION :
//
bool NCPopupSearch::postAgain()
{
    if ( ! postevent.widget )
	return false;

    postevent.result = YCPNull();
	
    YCPValue currentId =  dynamic_cast<YWidget *>(postevent.widget)->id();
    
    if ( currentId->compare( PkgNames::Cancel () ) == YO_EQUAL )
    {
	// set postevent.result to Null (checked in PackageSelector.cc)
	postevent.result = YCPNull();
    }
    else if ( currentId->compare( PkgNames::OkButton () ) == YO_EQUAL )
    {
	YCPValue value = YCPNull();
	bool ignore = true;
	bool descr = false;
	
	// get the search expression
	postevent.result =  getSearchExpression();

	if ( ignoreCase )
	{
	    value = ignoreCase->getValue();

	    // ignore case is not selected
	    if ( !value.isNull() && value->asBoolean()->toString() == "false" )
	    {
		ignore = false;
	    }
	}
	if ( checkDescr )
	{
	    value = checkDescr->getValue();

            // check description is selected 
	    if ( !value.isNull() && value->asBoolean()->toString() == "true" )
	    {
		descr = true;
	    }
	}

	// fill the package list with packages matching the search expression	
	packager->fillSearchList( postevent.result->asString(),
				  ignore,
				  descr  );
    }
    
    if ( postevent == NCursesEvent::button || postevent == NCursesEvent::cancel )
    {
	// return false means: close the popup dialog
	return false;
    }
    return true;
}

