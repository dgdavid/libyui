/*
  |****************************************************************************
  |
  | Copyright (c) 2000 - 2012 Novell, Inc.
  | All Rights Reserved.
  |
  | This program is free software; you can redistribute it and/or
  | modify it under the terms of version 2 of the GNU General Public License as
  | published by the Free Software Foundation.
  |
  | This program is distributed in the hope that it will be useful,
  | but WITHOUT ANY WARRANTY; without even the implied warranty of
  | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the
  | GNU General Public License for more details.
  |
  | You should have received a copy of the GNU General Public License
  | along with this program; if not, contact Novell, Inc.
  |
  | To contact Novell about this file by physical or electronic mail,
  | you may find current contact information at www.novell.com
  |
  |****************************************************************************
*/



 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////   __/\\\\\\_____________/\\\__________/\\\________/\\\___/\\\________/\\\___/\\\\\\\\\\\_           ////
 ////    _\////\\\____________\/\\\_________\///\\\____/\\\/___\/\\\_______\/\\\__\/////\\\///__          ////
 ////     ____\/\\\______/\\\__\/\\\___________\///\\\/\\\/_____\/\\\_______\/\\\______\/\\\_____         ////
 ////      ____\/\\\_____\///___\/\\\_____________\///\\\/_______\/\\\_______\/\\\______\/\\\_____        ////
 ////       ____\/\\\______/\\\__\/\\\\\\\\\_________\/\\\________\/\\\_______\/\\\______\/\\\_____       ////
 ////        ____\/\\\_____\/\\\__\/\\\////\\\________\/\\\________\/\\\_______\/\\\______\/\\\_____      ////
 ////         ____\/\\\_____\/\\\__\/\\\__\/\\\________\/\\\________\//\\\______/\\\_______\/\\\_____     ////
 ////          __/\\\\\\\\\__\/\\\__\/\\\\\\\\\_________\/\\\_________\///\\\\\\\\\/_____/\\\\\\\\\\\_    ////
 ////           _\/////////___\///___\/////////__________\///____________\/////////______\///////////__   ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                 widget abstraction library providing Qt, GTK and ncurses frontends                  ////
 ////                                                                                                     ////
 ////                                   3 UIs for the price of one code                                   ////
 ////                                                                                                     ////
 ////                                       ***  core library  ***                                        ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                                                     ////
 ////                                                                              (C) SUSE Linux GmbH    ////
 ////                                                                                                     ////
 ////                                                              libYUI-AsciiArt (C) 2012 Björn Esser   ////
 ////                                                                                                     ////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-/

  File:		YImage.cc

  Author:	Stefan Hundhammer <sh@suse.de>

/-*/


#include "YImage.h"
#include "YBothDim.h"


struct YImagePrivate
{
    /**
     * Constructor.
     **/
    YImagePrivate( const std::string & imageFileName, bool animated )
	: imageFileName( imageFileName )
	, animated( animated )
	, autoScale( false )
    {
	zeroSize.hor  = false;
	zeroSize.vert = false;
    }


    std::string		imageFileName;
    bool		animated;
    YBothDim<bool>	zeroSize;
    bool		autoScale;
};




YImage::YImage( YWidget *		parent,
		const std::string &	imageFileName,
		bool			animated )
    : YWidget( parent )
    , priv( new YImagePrivate( imageFileName, animated ) )
{
    YUI_CHECK_NEW( priv );
}


YImage::~YImage()
{
    // NOP
}


std::string YImage::imageFileName() const
{
    return priv->imageFileName;
}


bool YImage::animated() const
{
    return priv->animated;
}


void YImage::setImage( const std::string & imageFileName, bool animated )
{
    priv->imageFileName = imageFileName;
    priv->animated	= animated;
}


bool YImage::hasZeroSize( YUIDimension dim ) const
{
    return priv->zeroSize[ dim ];
}


void YImage::setZeroSize( YUIDimension dim, bool zeroSize )
{
    priv->zeroSize[ dim ] = zeroSize;
    setStretchable( dim, zeroSize );
}


bool YImage::autoScale() const
{
    return priv->autoScale;
}


void YImage::setAutoScale( bool autoScale )
{
    priv->autoScale = autoScale;
}
