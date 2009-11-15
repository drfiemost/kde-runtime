#ifndef oxygencomboboxdata_h
#define oxygencomboboxdata_h

//////////////////////////////////////////////////////////////////////////////
// oxygencomboboxdata.h
// data container for QComboBox transition
// -------------------
//
// Copyright (c) 2009 Hugo Pereira Da Costa <hugo@oxygen-icons.org>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//////////////////////////////////////////////////////////////////////////////

#include "oxygentransitiondata.h"

#include <QtCore/QString>
#include <QtCore/QBasicTimer>
#include <QtCore/QTimerEvent>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>

namespace Oxygen
{

    //! generic data
    class ComboBoxData: public TransitionData
    {

        Q_OBJECT

        public:

        //! constructor
        ComboBoxData( QObject*, QComboBox*, int );

        //! destructor
        virtual ~ComboBoxData( void )
        {}

        protected:

        //! timer event
        void timerEvent( QTimerEvent* );

        //! target rect
        /*! return rect corresponding to the area to be updated when animating */
        QRect targetRect( void ) const
        { return target_ ? target_.data()->rect().adjusted( 5, 5, -5, -5 ):QRect(); }

        protected slots:

        //! triggerd when comboBox edit text is changed
        virtual void textChanged( void );

        //! triggerd when comboBox text selection is changed
        virtual void selectionChanged( void )
        { timer_.start( 50, this ); }

        //! triggered when item is activated in combobox
        virtual void  indexChanged( void );

        //! initialize animation
        virtual bool initializeAnimation( void );

        //! animate
        virtual bool animate( void );

        private:

        //! timer
        QBasicTimer timer_;

        //! target
        QPointer<QComboBox> target_;

        //! line editor
        QPointer<QLineEdit> lineEdit_;

    };

}

#endif
