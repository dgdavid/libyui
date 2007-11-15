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

  File:	      YQIntField.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#ifndef YQIntField_h
#define YQIntField_h

#include <qlabel.h>
#include <qvbox.h>

#include "YIntField.h"


class YQWidgetCaption;
class QSpinBox;


class YQIntField : public QVBox, public YIntField
{
    Q_OBJECT

public:

    /**
     * Constructor.
     **/
    YQIntField( YWidget *	parent,
		const string &	label,
		int 		minValue,
		int 		maxValue,
		int 		initialValue );

    /**
     * Destructor.
     **/
    virtual ~YQIntField();

    /**
     * Get the current value (the number entered by the user or set from the
     * outside) of this IntField.
     *
     * Implemented from YIntField.
     **/
    virtual int value();

protected:

    /**
     * Set the current value (the number entered by the user or set from the
     * outside) of this IntField. 'val' is guaranteed to be between minValue
     * and maxValue; no further checks are required.
     *
     * Implemented from YIntField.
     **/
    virtual void setValueInternal( int val );

public:

    /**
     * Set the label (the caption above the input field).
     *
     * Reimplemented from YIntField.
     **/
    virtual void setLabel( const string & label );

    /**
     * Sets the widget's enabled state.
     *
     * Inherited from YWidget.
     **/
    virtual void setEnabled( bool enabled );

    /**
     * Preferred width of the widget.
     *
     * Reimplemented from YWidget.
     **/
    virtual int preferredWidth();

    /**
     * Preferred height of the widget.
     *
     * Reimplemented from YWidget.
     **/
    virtual int preferredHeight();

    /**
     * Set the new size of the widget.
     *
     * Reimplemented from YWidget.
     **/
    virtual void setSize( int newWidth, int newHeight );

    /**
     * Accept the keyboard focus.
     *
     * Reimplemented from YWidget.
     **/
    virtual bool setKeyboardFocus();

    
signals:

    /**
     * Emitted when the value changes (regardless of the notify flag).
     **/
    void valueChanged( int newValue );


protected slots:

    /**
     * Slot for "value changed". This will send a ValueChanged event if
     * 'notify' is set.
     **/
    void valueChangedSlot( int newValue );


protected:

    YQWidgetCaption *	_caption;
    QSpinBox *		_qt_spinBox;
};


#endif // YQIntField_h
