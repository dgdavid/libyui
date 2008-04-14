/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:		YDialog.h

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YDialog_h
#define YDialog_h

#include "YSingleChildContainerWidget.h"
#include "YEvent.h"
#include <stack>

class YShortcutManager;
class YPushButton;
class YDialogPrivate;

// See YTypes.h for enum YDialogType and enum YDialogColorMode


class YDialog : public YSingleChildContainerWidget
{
protected:
    /**
     * Constructor.
     *
     * 'dialogType' is one of YMainDialog or YPopupDialog.
     *
     * 'colorMode' can be set to YDialogWarnColor to use very bright "warning"
     * colors or YDialogInfoColor to use more prominent, yet not quite as
     * bright as "warning" colors. Use both only very rarely.
     **/
    YDialog( YDialogType 	dialogType,
	     YDialogColorMode	colorMode = YDialogNormalColor );

    /**
     * Destructor.
     * Don't delete a dialog directly, use YDialog::deleteTopmostDialog()
     * or YDialog::destroy().
     **/
    virtual ~YDialog();

public:
    /**
     * Return a descriptive name of this widget class for logging,
     * debugging etc.
     **/
    virtual const char * widgetClass() const { return "YDialog"; }

    /**
     * Open a newly created dialog: Finalize it and make it visible
     * on the screen.
     *
     * Applications should call this once after all children are created.
     * If the application doesn't do this, it will be done automatically upon
     * the next call of YDialog::waitForEvent() (or related). This is OK if
     * YDialog::waitForEvent() is called immediately after creating the dialog
     * anyway. If it is not, the application might appear sluggish to the user.
     *
     * Derived classes are free to reimplement this, but they should call this
     * base class method in the new implementation.
     **/
    void open();

    /**
     * Return 'true' if open() has already been called for this dialog.
     **/
    bool isOpen() const;

    /**
     * Wait for a user event. In most cases, this means waiting until the user
     * has clicked on a button in this dialog. If any widget has its 'notify'
     * flag set (`opt(`notify) in YCP, setNotify( true ) in C++), an action on
     * such a widget will also make waitForEvent() return.
     *
     * If the specified timeout elapses without any user event, a YTimeoutEvent
     * will be returned. 0 means no timeout (wait forever).
     *
     * If open() has not been called for this dialog until now,
     * it is called now.
     *
     * Ownership of the event is transferred to the caller, i.e. the caller is
     * responsible for deleting it after use.
     *
     * If this dialog is not the topmost dialog, an exception is thrown.
     **/
    YEvent * waitForEvent( int timeout_millisec = 0 );

    /**
     * Check if a user event is pending. If there is one, return it.
     * If there is none, do not wait for one - return 0.
     *
     * If open() has not been called for this dialog until now,
     * it is called now.
     *
     * Ownership of the event is transferred to the caller, i.e. the caller is
     * responsible for deleting it after use.
     *
     * If this dialog is not the topmost dialog, an exception is thrown.
     **/ 
    YEvent * pollEvent();

    /**
     * Return 'true' if this dialog is the topmost dialog.
     **/
    bool isTopmostDialog() const;

    /**
     * Close and delete this dialog (and all its children) if it is the topmost
     * dialog. If this is not the topmost dialog, this will throw an exception
     * if 'doThrow' is true (default).
     *
     * Remember that all pointers to the dialog and its children will be
     * invalid after this operation.
     *
     * This is intentionally not named close() since close() would not imply
     * that the dialog and its children are deleted.
     *
     * Returns 'true' upon success, 'false' upon failure.
     **/
    bool destroy( bool doThrow = true );

    /**
     * Delete the topmost dialog.
     *
     * Will throw a YUINoDialogException if there is no dialog and 'doThrow' is
     * 'true'.
     *
     * This is equivalent to YDialog::currentDialog()->destroy().
     *
     * Returns 'true' if there is another open dialog after deleting,
     * 'false' if there is none.
     **/
    static bool deleteTopmostDialog( bool doThrow = true );

    /**
     * Delete all open dialogs.
     **/
    static void deleteAllDialogs();

    /**
     * Delete all dialogs from the topmost to the one specified.
     **/
    static void deleteTo( YDialog * dialog );

    /**
     * Returns the number of currently open dialogs (from 1 on), i.e., the
     * depth of the dialog stack.
     **/
    static int openDialogsCount();

    /**
     * Return the current (topmost) dialog.
     *
     * If there is none, throw a YUINoDialogException if 'doThrow' is 'true'
     * and return 0 if 'doThrow' is false.
     **/
    static YDialog * currentDialog( bool doThrow = true );

    /**
     * Alias for currentDialog().
     **/
    static YDialog * topmostDialog( bool doThrow = true )
	{ return currentDialog( doThrow ); }

    /**
     * Set the initial dialog size, depending on dialogType:
     * YMainDialog dialogs get the UI's "default main window" size,
     * YPopupDialog dialogs use their content's preferred size.
     **/
    void setInitialSize();

    /**
     * Recalculate the layout of the dialog and of all its children after
     * children have been added or removed or if any of them changed its
     * preferred width of height.
     *
     * This is a very expensive operation. Call it only when really necessary.
     * YDialog::open() includes a call to YDialog::setInitialSize() which does
     * the same.
     *
     * The basic idea behind this function is to call it when the dialog
     * changed after it (and its children hierarchy) was initially created.
     **/
    void recalcLayout();

    /**
     * Return this dialog's type (YMainDialog / YPopupDialog /YWizardDialog).
     **/
    YDialogType dialogType() const;

    /**
     * Return 'true' if this dialog is a dialog of main dialog size:
     * YMainDialog or YWizardDialog. 
     **/
    bool isMainDialog();

    /**
     * Return this dialog's color mode.
     **/
    YDialogColorMode colorMode() const;

    /**
     * Checks the keyboard shortcuts of widgets in this dialog unless shortcut
     * checks are postponed or 'force' is 'true'.
     *
     * A forced shortcut check resets postponed checking.
     **/
    void checkShortcuts( bool force = false );

    /**
     * From now on, postpone keyboard shortcut checks - i.e. normal (not
     * forced) checkKeyboardShortcuts() will do nothing.  Reset this mode by
     * forcing a shortcut check with checkKeyboardShortcuts( true ).
     **/
    void postponeShortcutCheck();

    /**
     * Return whether or not shortcut checking is currently postponed.
     **/
    bool shortcutCheckPostponed() const;

    /**
     * Return this dialog's default button: The button that is activated when
     * the user hits [Return] anywhere in this dialog. Note that this is not
     * the same as the button that currently has the keyboard focus.
     *
     * This might return 0 if there is no default button.
     **/
    YPushButton * defaultButton() const;

    /**
     * Set this dialog's default button (the button that is activated when
     * the user hits [Return] anywhere in this dialog). 0 means no default
     * button.
     *
     * There should be no more than one default button in a dialog.
     *
     * Derived classes are free to overwrite this method, but they should
     * call this base class method in the new implementation.
     **/
    virtual void setDefaultButton( YPushButton * defaultButton );

    /**
     * Activate this dialog: Make sure that it is shown as the topmost dialog
     * of this application and that it can receive input.
     *
     * Derived classes are required to implement this.
     **/
    virtual void activate() = 0;

    /**
     * Show the specified text in a pop-up dialog with a local event loop.
     * This is useful for help texts.
     * 'richText' indicates if YRichText formatting should be applied.
     **/
    static void showText( const string & text, bool richText = false );


protected:

    /**
     * Internal open() method. This is called (exactly once during the life
     * time of the dialog) in open().
     *
     * Derived classes are required to implement this to do whatever is
     * necessary to make this dialog visible on the screen.
     **/
    virtual void openInternal() = 0;

    /**
     * Wait for a user event.
     *
     * Derived classes are required to implement this.
     **/
    virtual YEvent * waitForEventInternal( int timeout_millisec ) = 0;
    
    /**
     * Check if a user event is pending. If there is one, return it.
     * If there is none, do not wait for one - return 0.
     *
     * Derived classes are required to implement this.
     **/
    virtual YEvent * pollEventInternal() = 0;

    /**
     * Filter out invalid events: Return 0 if the event does not belong to this
     * dialog or the unchanged event if it does.
     **/
    YEvent * filterInvalidEvents( YEvent * event );

    /**
     * Stack holding all currently existing dialogs.
     **/
    static std::stack<YDialog *> _dialogStack;

private:

    ImplPtr<YDialogPrivate> priv;
};


#endif // YDialog_h