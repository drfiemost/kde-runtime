/* This file is part of the KDE project
   Copyright (C) 2001 Holger Freyther <freyther@yahoo.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; version 2
   of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

//#include "image_plugin.moc"

#include <kdebug.h>
#include <kgenericfactory.h>
#include <kiconloader.h>
#include <renamedlgplugin.h>
#include <kio/renamedlg.h>
#include <qlabel.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qstringlist.h>
#include <kio/global.h>
#include <qlayout.h>

#include <sys/types.h>

#include "imagevisualizer.h"

class KTestMenu : public RenameDlgPlugin{
public:
  KTestMenu( QDialog *dialog, const char *name, const QStringList & );
  ~KTestMenu();
  virtual bool initialize( KIO::RenameDlg_Mode /*mod*/, const QString &/*_src*/, const QString &/*_dest*/,
		  const QString &/*mimeSrc*/,
		  const QString &/*mimeDest*/,
		  KIO::filesize_t /*sizeSrc*/,
		  KIO::filesize_t /*sizeDest*/,
		  time_t /*ctimeSrc*/,
		  time_t /*ctimeDest*/,
		  time_t /*mtimeSrc*/,
		  time_t /*mtimeDest*/ );
};

KTestMenu::KTestMenu( QDialog *dialog, const char *name, const QStringList &list ) : RenameDlgPlugin( dialog, name, list) {
  qWarning("loaded" );  
};
KTestMenu::~KTestMenu()
{
qWarning("byebye and cu all" );
}
bool KTestMenu::initialize( KIO::RenameDlg_Mode mode, const QString &_src, const QString &_dest,
		  const QString &mimeSrc,
		  const QString &mimeDest,
		  KIO::filesize_t sizeSrc,
		  KIO::filesize_t sizeDest,
		  time_t ctimeSrc,
		  time_t ctimeDest,
		  time_t mtimeSrc,
		      time_t mtimeDest ) {
  qWarning("%s", _src.latin1() );
  qWarning("%s", _dest.latin1() );
  qWarning("%s", mimeSrc.latin1() );
  qWarning("%s", mimeDest.latin1() );

  QGridLayout *lay = new QGridLayout(this, 2, 3, 5  );
qWarning("ABC3456: %d", mode );
 if( mode & KIO::M_OVERWRITE ){
   QLabel *label = new QLabel(this );
   label->setText(i18n("You want to overwrite the left picture with the one on the right.") );
   label->adjustSize();
   lay->addMultiCellWidget(label, 1, 1, 0, 2,  Qt::AlignHCenter  );
qWarning("overwrite" );
 adjustSize();
 }
 ImageVisualizer *left= new ImageVisualizer(this, "Visualizer Left", _dest );
 ImageVisualizer *right = new ImageVisualizer( this, "Visualizer Right", _src );
 lay->addWidget(left, 2, 0 );
 lay->addWidget(right, 2, 2 );
 adjustSize();
  return true;
}

typedef KGenericFactory<KTestMenu, QDialog> KImageFactory;
K_EXPORT_COMPONENT_FACTORY( librenimageplugin, KImageFactory("imagerename_plugin") );
