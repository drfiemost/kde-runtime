/* Name: smbshare.h

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


/*
 * NOTE: This source file was created using tab size = 2.
 * Please respect that setting in case of modifications.
 */

#ifndef __INC_SMBSHARE_H__
#define __INC_SMBSHARE_H__

#include "common.h"
#include "array.h"
#include "treeitem.h"
#include "smbfile.h"

class CSMBShareInfo
{
public:
    CSMBShareInfo() {}

	CSMBShareInfo& operator=(const CSMBShareInfo& other)
	{
		m_ShareName = (LPCSTR)other.m_ShareName
#ifdef QT_20
	.latin1()
#endif
                ;
		m_ShareType = (LPCSTR)other.m_ShareType
#ifdef QT_20
	.latin1()
#endif
                ;
		m_Comment = (LPCSTR)other.m_Comment
#ifdef QT_20
	.latin1()
#endif
                ;
		return *this;
	}

/*private:*/
    QString m_ShareName;
    QString m_ShareType;
    QString m_Comment;
};

class CShareArray : public CVector<CSMBShareInfo,CSMBShareInfo&>
{
};

class CShareItem : public CNetworkFileContainer, public CSMBShareInfo
{
public:
	CShareItem(CListView *parent, CListViewItem *pLogicalParent, CSMBShareInfo *pInfo);
	CShareItem(CNetworkTreeItem *parent, CListViewItem *pLogicalParent, CSMBShareInfo *pInfo);

	QTTEXTTYPE text(int column) const;

	virtual BOOL ItemAcceptsDrops()
	{
		return TRUE;
	}

	void Init()
	{
		m_nCredentialsIndex = -1;
		InitPixmap();
	}

	QPixmap *Pixmap()
	{
		return Pixmap(FALSE);
	}

	QPixmap *BigPixmap()
	{
		return Pixmap(TRUE);
	}

	QPixmap *Pixmap(BOOL bIsBig);

	CItemKind Kind()
	{
		return keShareItem;
	}
};

#endif /* __INC_SMBSHARE_H__ */

