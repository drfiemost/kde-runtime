#include <qstringlist.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qlistbox.h>
#include <qwhatsthis.h>
#include <qlabel.h>

#include <kdebug.h>
#include <kdialog.h>
#include <klineedit.h>
#include <klocale.h>
#include <kopenwith.h>
#include <kmimetype.h>
#include <kservice.h>
#include <kservicetype.h>
#include <kuserprofile.h>

#include "kservicelistwidget.h"
#include "typeslistitem.h"

class KServiceListItem : public QListBoxText
{
public:
	KServiceListItem( QString &desktopPath );

	QString desktopPath;
};

KServiceListItem::KServiceListItem( QString &_desktopPath )
	: QListBoxText(), desktopPath(_desktopPath)
{
    KService::Ptr pService = KService::serviceByDesktopPath( _desktopPath );

    ASSERT(pService);

    setText( pService->name() );
}

KServiceListWidget::KServiceListWidget(int kind, QWidget *parent, const char *name)
  : QGroupBox( kind == SERVICELIST_APPLICATIONS ? i18n("Application Preference Order")
               : i18n("Services Preference Order"), parent, name )
{
  QWidget * gb = this;
  QGridLayout * grid = new QGridLayout(gb, 5, 2, KDialog::marginHint(),
                         KDialog::spacingHint());
  grid->addRowSpacing(0, fontMetrics().lineSpacing());
//  grid->setRowStretch(3, 1);

  servicesLB = new QListBox(gb);
  connect(servicesLB, SIGNAL(highlighted(int)), SLOT(enableMoveButtons(int)));
  grid->addMultiCellWidget(servicesLB, 1, 4, 0, 0);

  QString wtstr = i18n("This is a list of applications associated with files of the selected"
    " file type. This list is shown in Konqueror's context menus when you select"
    " \"Open with...\". If more than one application is associated with this file type,"
    " then the list is ordered by priority with the uppermost item taking precedence"
    " over the others.");
  QWhatsThis::add( gb, wtstr );
  QWhatsThis::add( servicesLB, wtstr );

  servUpButton = new QPushButton(i18n("Move &Up"), gb);
  servUpButton->setEnabled(false);
  connect(servUpButton, SIGNAL(clicked()), SLOT(promoteService()));
  grid->addWidget(servUpButton, 1, 1);

  QWhatsThis::add( servUpButton, i18n("Assigns a higher priority to the selected\n"
                              "application, moving it up in the list. Note:  This\n"
                              "only affects the selected application if the file type is\n"
			      "associated with more than one application."));

  servDownButton = new QPushButton(i18n("Move &Down"), gb);
  servDownButton->setEnabled(false);
  connect(servDownButton, SIGNAL(clicked()), SLOT(demoteService()));
  grid->addWidget(servDownButton, 2, 1);

  QWhatsThis::add( servDownButton, i18n("Assigns a lower priority to the selected\n"
			  "application, moving it down on the list.  Note:  This \n"
			  "only affects the selected application if the file type is\n"
			  "associated with more than one application."));

  servNewButton = new QPushButton(i18n("Add..."), gb);
  servNewButton->setEnabled(false);
  connect(servNewButton, SIGNAL(clicked()), SLOT(addService()));
  grid->addWidget(servNewButton, 3, 1);

  QWhatsThis::add( servNewButton, i18n( "Add a new application for this file type." ) );

  servRemoveButton = new QPushButton(i18n("Remove"), gb);
  servRemoveButton->setEnabled(false);
  connect(servRemoveButton, SIGNAL(clicked()), SLOT(removeService()));
  grid->addWidget(servRemoveButton, 4, 1);

  QWhatsThis::add( servRemoveButton, i18n( "Remove the selected application from the list." ) );
}

void KServiceListWidget::setTypeItem( TypesListItem * item )
{
  m_item = item;
  servNewButton->setEnabled(true);
  // will need a selection
  servUpButton->setEnabled(false);
  servDownButton->setEnabled(false);
  servRemoveButton->setEnabled(false);
  servicesLB->clear();
  servicesLB->setEnabled(false);

  if ( item )
  {
    QStringList services = item->defaultServices();

    if (services.count() == 0) {
      servicesLB->insertItem("None");
    } else {
      for ( QStringList::Iterator it = services.begin();
            it != services.end(); it++ )
      {
        servicesLB->insertItem( new KServiceListItem(*it) );
      }
      servicesLB->setEnabled(true);
    }
  }
}

void KServiceListWidget::promoteService()
{
  if (!servicesLB->isEnabled()) {
    kapp->beep();
    return;
  }

  unsigned int selIndex = servicesLB->currentItem();
  if (selIndex == 0) {
    kapp->beep();
    return;
  }

  QListBoxItem *selItem = servicesLB->item(selIndex);
  servicesLB->takeItem(selItem);
  servicesLB->insertItem(selItem, selIndex-1);
  servicesLB->setCurrentItem(selIndex - 1);

  updatePreferredServices();

  emit changed(true);
}

void KServiceListWidget::demoteService()
{
  if (!servicesLB->isEnabled()) {
    kapp->beep();
    return;
  }

  unsigned int selIndex = servicesLB->currentItem();
  if (selIndex == servicesLB->count() - 1) {
    kapp->beep();
    return;
  }

  QListBoxItem *selItem = servicesLB->item(selIndex);
  servicesLB->takeItem(selItem);
  servicesLB->insertItem(selItem, selIndex+1);
  servicesLB->setCurrentItem(selIndex + 1);

  updatePreferredServices();

  emit changed(true);
}

void KServiceListWidget::addService()
{
  if (!m_item)
      return;

  KOpenWithDlg dlg(m_item->name(), QString::null, 0L);
  if (dlg.exec() == false)
    return;

  KService::Ptr service = dlg.service();

  ASSERT(service);

  // check if it is a duplicate entry
  for (unsigned int index = 0; index < servicesLB->count(); index++)
    if (servicesLB->text(index) == service->name())
      return;

  // if None is the only item, then there currently is no default
  if (servicesLB->text(0) == "None") {
      servicesLB->removeItem(0);
      servicesLB->setEnabled(true);
  }
  QString desktopPath = service->desktopEntryPath();

  servicesLB->insertItem( new KServiceListItem(desktopPath) );

  updatePreferredServices();

  emit changed(true);
}

void KServiceListWidget::removeService()
{
  int selected = servicesLB->currentItem();

  if ( selected >= 0 ) {
    servicesLB->removeItem( selected );
    updatePreferredServices();

    emit changed(true);
  }

  if ( servicesLB->currentItem() == -1 )
    servRemoveButton->setEnabled(false);

}

void KServiceListWidget::updatePreferredServices()
{
  if (!m_item)
    return;
  QStringList sl;
  unsigned int count = servicesLB->count();

  for (unsigned int i = 0; i < count; i++) {
    KServiceListItem *sli = (KServiceListItem *) servicesLB->item(i);
    sl.append( sli->desktopPath );
  }
  m_item->setDefaultServices(sl);
}

void KServiceListWidget::enableMoveButtons(int index)
{
  if (servicesLB->count() <= 1)
  {
    servUpButton->setEnabled(false);
    servDownButton->setEnabled(false);
  }
  else if ((uint) index == (servicesLB->count() - 1))
  {
    servUpButton->setEnabled(true);
    servDownButton->setEnabled(false);
  }
  else if (index == 0)
  {
    servUpButton->setEnabled(false);
    servDownButton->setEnabled(true);
  }
  else
  {
    servUpButton->setEnabled(true);
    servDownButton->setEnabled(true);
  }

  servRemoveButton->setEnabled(true);
}

#include "kservicelistwidget.moc"
