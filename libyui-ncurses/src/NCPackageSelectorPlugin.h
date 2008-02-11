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

  File:	      NCPackageSelectorPlugin.h

  Author:     Gabriele Mohr <gs@suse.de>

/-*/

// -*- c++ -*-

#ifndef NCPackageSelectorPlugin_h
#define NCPackageSelectorPlugin_h

#include <YPackageSelectorPlugin.h>

class YDialog;
class YEvent;


/**
 * Simplified access to the ncurses UI's package selector plugin.
 **/
class NCPackageSelectorPlugin: public YPackageSelectorPlugin
{
public:

    /**
     * Constructor: Load the plugin library for the NCurses package selector.
     **/
    NCPackageSelectorPlugin();

    /**
     * Destructor. Calls dlclose() which will unload the plugin library if it
     * is no longer used, i.e. if the reference count dlopen() uses reaches 0.
     **/
    virtual ~NCPackageSelectorPlugin();

    /**
     * Create a package selector.
     * Implemented from YPackageSelectorPlugin.
     *
     * This might return 0 if the plugin lib could not be loaded or if the
     * appropriate symbol could not be located in the plugin lib. 
     **/
    virtual YPackageSelector * createPackageSelector( YWidget *		parent,
						      long		modeFlags );

    /**
     * Fills the PackageSelector widget (runs the package selection).
     */
    virtual YEvent * runPkgSelection( YDialog *currentDialog,
				      YWidget * packageSelector );

    /**
     * Create a special widget
     */
    virtual YWidget * createPkgSpecial( YWidget *parent,
					const string &subwidget );
};



#endif // NCPackageSelectorPlugin_h
