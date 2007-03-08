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

   File:       NCDialog.h

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

/-*/
#ifndef NCDialog_h
#define NCDialog_h

#include <iosfwd>

#include "YDialog.h"
#include "NCWidget.h"


class NCDialog;
class NCPopupInfo;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : NCDialog
//
//	DESCRIPTION :
//
class NCDialog : public YDialog, public NCWidget {

  friend std::ostream & operator<<( std::ostream & STREAM, const NCDialog & OBJ );
  friend std::ostream & operator<<( std::ostream & STREAM, const NCDialog * OBJ );

  NCDialog & operator=( const NCDialog & );
  NCDialog            ( const NCDialog & );

  private:

    typedef tnode<NCWidget *> * (tnode<NCWidget *>::* SeekDir)( const bool );

    NCWidget & GetNormal( NCWidget & startwith, SeekDir Direction );
    void       Activate( SeekDir Direction );

    void _init( const YWidgetOpt & opt );

    void _init_size();

  protected:

    virtual const char * location() const { return "NCDialog"; }

  private:

    NCursesUserPanel<NCDialog> * pan;
    NCstyle::StyleSet            mystyleset;
    const NCstyle::Style *       dlgstyle;

    unsigned inMultiDraw_i;

    bool            active;
    NCWidget *const wActive;

    NCursesEvent pendingEvent;
    YEvent::EventReason eventReason;

    NCPopupInfo *helpPopup;

    // wrapper for wHandle... calls in processInput()
    NCursesEvent getInputEvent( wint_t ch );
    NCursesEvent getHotkeyEvent( wint_t key );

  private:

    void grabActive( NCWidget * nactive );
    virtual void grabNotify( NCWidget * mgrab );
    virtual bool wantFocus( NCWidget & ngrab );

    virtual void wCreate( const wrect & newrect );
    virtual void wMoveTo( const wpos & newpos );
    virtual void wDelete();
    virtual void wRedraw();
    virtual void wRecoded();
    virtual void wUpdate( bool forced_br = false );
    void doUpdate() { wUpdate( true ); }

    NCWidget & GetNextNormal( NCWidget & startwith );
    NCWidget & GetPrevNormal( NCWidget & startwith );

    bool Activate( NCWidget & nactive );
    void Activate();
    void Deactivate();
    void ActivateNext();
    void ActivatePrev();

    bool ActivateByKey( int key );

    void processInput( int timeout_millisec );

    bool describeFunctionKeys( string & helpText );

    wint_t getinput();		// get the input (respect terminal encoding)

    bool flushTypeahead();
    
  protected:

    wint_t getch( int timeout_millisec = -1 );

    virtual NCursesEvent wHandleInput( wint_t ch );
    virtual NCursesEvent wHandleHotkey( wint_t key );

    virtual void startMultipleChanges();
    virtual void doneMultipleChanges();

  public:

    NCDialog( const YWidgetOpt & opt );
    virtual ~NCDialog();

    void showDialog();
    void closeDialog();

    void activate( const bool newactive );
    bool isActive() const { return active; }

    void idleInput();

    NCursesEvent userInput( int timeout_millisec = -1 );
    NCursesEvent pollInput();

    virtual long nicesize( YUIDimension dim );
    virtual void setSize( long newwidth, long newheight );

  protected:

    enum NCDopts {
      DEFAULT = 0x00,
      POPUP   = 0x01,
      NOBOX   = 0x10
    };

    typedef unsigned NCDoptflag;

    NCDoptflag ncdopts;
    wpos       popedpos;
    bool       hshaddow;
    bool       vshaddow;

    NCDialog( const YWidgetOpt & opt, const wpos at, const bool boxed = true );

    bool isPopup() const { return  (ncdopts & POPUP); }
    bool isBoxed() const { return !(ncdopts & NOBOX); }

    virtual void initDialog();

    virtual const NCstyle::Style & wStyle() const {
      return dlgstyle ? *dlgstyle : NCurses::style()[NCstyle::DefaultStyle];
    }

    virtual void setEnabling( bool do_bv ) { /*NOP*/ }

  private:

    friend class NCurses;
    bool getInvisible();
    bool getVisible();
    void resizeEvent();
};

///////////////////////////////////////////////////////////////////

#endif // NCDialog_h
