/* Name: MessageBoxFourData.h

   Description: This file is a part of the libmwn library.

   Author:	Oleg Noskov (olegn@corel.com)

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.


*/

#ifndef CMessageBoxFourData_included
#define CMessageBoxFourData_included

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>

class CMessageBoxFourData : public QDialog
{
    Q_OBJECT

public:

    CMessageBoxFourData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~CMessageBoxFourData();

public slots:


protected slots:

    virtual void OnButton1();
    virtual void OnButton3();
    virtual void OnButton2();
    virtual void OnButton4();

protected:
    QLabel* m_pIcon;
    QLabel* m_pText;
    QPushButton* m_pButton1;
    QPushButton* m_pButton2;
    QPushButton* m_pButton3;
    QPushButton* m_pButton4;

};

#endif // CMessageBoxFourData_included
