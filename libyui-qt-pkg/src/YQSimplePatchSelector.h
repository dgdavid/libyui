/**************************************************************************
Copyright (C) 2000 - 2010 Novell, Inc.
All Rights Reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

**************************************************************************/


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

  File:	      YQSimplePatchSelector.h

  Author:     Stefan Hundhammer <shundhammer.de>

/-*/


#ifndef YQSimplePatchSelector_h
#define YQSimplePatchSelector_h

#include <QColor>

#include "YQPackageSelectorBase.h"


class QPushButton;

class YQPkgPatchFilterView;
class YQPkgPatchList;
class YQWizard;


class YQSimplePatchSelector : public YQPackageSelectorBase
{
    Q_OBJECT

public:

    YQSimplePatchSelector( YWidget * parent, long modeFlags );


protected slots:

    /**
     * User clicked on "Details..." - start the detailed package selection.
     * This will return from UI::RunPkgSelection() with `details .
     **/
    void detailedPackageSelection();

    /**
     * Debugging
     **/
    void debugTrace();


protected:

    // Layout methods - create and layout widgets

    void	basicLayout();
    void	layoutButtons	( QWidget * parent );

    /**
     * Find the wizard in the current dialog, if there is any.
     * Returns 0 if there is none.
     **/
    YQWizard *	findWizard() const;

    /**
     * Establish Qt signal / slot connections.
     **/
    void makeConnections();


    // Data members

    YQPkgPatchFilterView *	_patchFilterView;
    YQPkgPatchList *		_patchList;
    YQWizard *			_wizard;
};



#endif // YQSimplePatchSelector_h
