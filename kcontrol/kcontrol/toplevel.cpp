/*
  Copyright (c) 1999 Matthias Hoelzer-Kluepfel <hoelzer@kde.org>
  Copyright (c) 2000 Matthias Elter <elter@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <kapp.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <kconfig.h>
#include <kdebug.h>
#include <kbugreport.h>
#include <kaboutdata.h>

#include <qsplitter.h>
#include <qtabwidget.h>

#include <kaction.h>
#include <kstdaction.h>
#include <klocale.h>
#include <kmenubar.h>


#include "indexwidget.h"
#include "searchwidget.h"
#include "helpwidget.h"
#include "dockcontainer.h"
#include "aboutwidget.h"
#include "modules.h"
#include "proxywidget.h"
#include "global.h"
#include "modulemenu.h"
#include <stdio.h>

#include "toplevel.h"
#include "toplevel.moc"


TopLevel::TopLevel(const char* name)
  : KTMainWindow( name )
  , _active(0), dummyAbout(0)
{
  setPlainCaption(i18n("KDE Control Center"));

  // read settings
  KConfig *config = KGlobal::config();
  config->setGroup("Index");
  QString viewmode = config->readEntry("ViewMode", "Icon");

  if (viewmode == "Tree")
    KCGlobal::setViewMode(Tree);
  else
    KCGlobal::setViewMode(Icon);

  QString size = config->readEntry("IconSize", "Medium");
  if (size == "Small")
    KCGlobal::setIconSize(Small);
  else if (size == "Large")
    KCGlobal::setIconSize(Large);
  else
    KCGlobal::setIconSize(Medium);

  // initialize the entries
  _modules = new ConfigModuleList();
  _modules->readDesktopEntries();

  // create the splitter
  _splitter = new QSplitter(this);

  // create the left hand side (the tab view)
  _tab = new QTabWidget(_splitter);

  // index tab
  _indextab = new IndexWidget(_modules, _tab);
  connect(_indextab, SIGNAL(moduleActivated(ConfigModule*)),
		  this, SLOT(moduleActivated(ConfigModule*)));
  _tab->addTab(_indextab, i18n("In&dex"));

  // search tab
  _searchtab = new SearchWidget(_tab);
  _searchtab->populateKeywordList(_modules);
  connect(_searchtab, SIGNAL(moduleSelected(const QString&)),
		  this, SLOT(activateModule(const QString&)));

  _tab->addTab(_searchtab, i18n("S&earch"));

  // help tab
  _helptab = new HelpWidget(_tab);
  _tab->addTab(_helptab, i18n("Hel&p"));

  // set a reasonable resize mode
  _splitter->setResizeMode(_tab, QSplitter::KeepSize);

  // set up the right hand side (the docking area)
  _dock = new DockContainer(_splitter);
  connect(_dock, SIGNAL(newModule(const QString&, const QString&)),
		  this, SLOT(newModule(const QString&, const QString&)));

  // insert the about widget
  AboutWidget *aw = new AboutWidget(this);
  _dock->setBaseWidget(aw);

  // set the main view
  setView(_splitter);

  // initialize the GUI actions
  setupActions();

  // activate defaults
  if (KCGlobal::viewMode() == Tree)   {
    activateTreeView();
    tree_view->setChecked(true);
  }
  else {
    activateIconView();
    icon_view->setChecked(true);
  }
}

TopLevel::~TopLevel()
{
  KConfig *config = KGlobal::config();
  config->setGroup("Index");
  if (KCGlobal::viewMode() == Tree)
    config->writeEntry("ViewMode", "Tree");
  else
    config->writeEntry("ViewMode", "Icon");

  switch (KCGlobal::iconSize())
    {
    case Small:
      config->writeEntry("IconSize", "Small");
      break;
    case Medium:
      config->writeEntry("IconSize", "Medium");
      break;
    case Large:
      config->writeEntry("IconSize", "Large");
      break;
    default:
      config->writeEntry("IconSize", "Medium");
      break;
    }

  config->sync();
}

void TopLevel::setupActions()
{
  KStdAction::quit(kapp, SLOT(quit()), actionCollection());

  icon_view = new KRadioAction
    (i18n("&Icon View"), 0, this, SLOT(activateIconView()),
     actionCollection(), "activate_iconview");
  icon_view->setExclusiveGroup( "viewmode" );

  tree_view = new KRadioAction
    (i18n("&Tree View"), 0, this, SLOT(activateTreeView()),
     actionCollection(), "activate_treeview");
  tree_view->setExclusiveGroup( "viewmode" );

  icon_small = new KRadioAction
    (i18n("&Small"), 0, this, SLOT(activateSmallIcons()),
     actionCollection(), "activate_smallicons");
  icon_small->setExclusiveGroup( "iconsize" );

  icon_medium = new KRadioAction
    (i18n("&Medium"), 0, this, SLOT(activateMediumIcons()),
     actionCollection(), "activate_mediumicons");
  icon_medium->setExclusiveGroup( "iconsize" );

  icon_large = new KRadioAction
    (i18n("&Large"), 0, this, SLOT(activateLargeIcons()),
     actionCollection(), "activate_largeicons");
  icon_large->setExclusiveGroup( "iconsize" );

  // I need to add this so that each module can get a bug reported,
  // and not just KControl

  report_bug=new KAction(i18n("&Report Bug..."),0,
			 this, SLOT(reportBug()), actionCollection(),
			 "help_report_bug");

  createGUI("kcontrolui.rc");

  // add menu with the modules
  ModuleMenu *menu = new ModuleMenu(_modules, this);
  menuBar()->insertItem(i18n("&Modules"), menu, -1, 2);
  connect(menu, SIGNAL(moduleActivated(ConfigModule*)),
	  this, SLOT(moduleActivated(ConfigModule*)));
}

void TopLevel::activateIconView()
{
  KCGlobal::setViewMode(Icon);
  _indextab->activateView(Icon);

  icon_small->setEnabled(true);
  icon_medium->setEnabled(true);
  icon_large->setEnabled(true);

  switch(KCGlobal::iconSize())
    {
    case Small:
      icon_small->setChecked(true);
      break;
    case Large:
      icon_large->setChecked(true);
      break;
    default:
      icon_medium->setChecked(true);
      break;
    }
}

void TopLevel::activateTreeView()
{
  KCGlobal::setViewMode(Tree);
  _indextab->activateView(Tree);

  icon_small->setEnabled(false);
  icon_medium->setEnabled(false);
  icon_large->setEnabled(false);
}

void TopLevel::activateSmallIcons()
{
  KCGlobal::setIconSize(Small);
  _indextab->reload();
}

void TopLevel::activateMediumIcons()
{
  KCGlobal::setIconSize(Medium);
  _indextab->reload();
}

void TopLevel::activateLargeIcons()
{
  KCGlobal::setIconSize(Large);
  _indextab->reload();
}

void TopLevel::newModule(const QString &name, const QString &quickhelp)
{
  QString cap = i18n("KDE Control Center");

  if (!name.isEmpty())
    cap += " - [" + name +"]";

  setPlainCaption(cap);

  _helptab->setText(quickhelp);
}

void TopLevel::moduleActivated(ConfigModule *module)
{
    _active=module;
    activateModule(module->name());
}

void TopLevel::showModule(QString desktopFile)
{
  // strip trailing ".desktop"
  int pos = desktopFile.find(".desktop");
  if (pos > 0)
    desktopFile = desktopFile.left(pos);

  // locate the desktop file
  QStringList files;
  files = KGlobal::dirs()->findAllResources("apps", "Settings/"+desktopFile+".desktop", TRUE);

  // show all matches
  QStringList::Iterator it;
  for (it = files.begin(); it != files.end(); ++it)
    {
      for (ConfigModule *mod = _modules->first(); mod != 0; mod = _modules->next())
		if (mod->fileName() == *it && mod != _active)
		  {
			// tell the index to display the module
			_indextab->makeVisible(mod);
			
			// tell the index to mark this module as loaded
			_indextab->makeSelected(mod);

			// dock it
            _dock->dockModule(mod);
            break;
	  }
    }
}

void TopLevel::activateModule(const QString& name)
{
  kdDebug() << "activate: " << name << endl;
  for (ConfigModule *mod = _modules->first(); mod != 0; mod = _modules->next())
	{
	  if (mod->name() == name)
		{
		  // tell the index to display the module
		  _indextab->makeVisible(mod);

		  // tell the index to mark this module as loaded
		  _indextab->makeSelected(mod);
		
		  // dock it
		  _dock->dockModule(mod);
		  break;
		}
	}
  report_bug->setText(i18n("Report Bug on Module %1...").arg(name));
}

void TopLevel::deleteDummyAbout() {
  delete dummyAbout;
  dummyAbout = 0;
}
void TopLevel::reportBug()
{
  // this assumes the user only opens one bug report at a time
  static char buffer[30];

    dummyAbout = 0;
    bool deleteit = false;

    if (!_active) // report against kcontrol
        dummyAbout = const_cast<KAboutData*>(KGlobal::instance()->aboutData());
    else {
	if (_active->aboutData())
	    dummyAbout = const_cast<KAboutData*>(_active->aboutData());
	else {
	  sprintf(buffer, "kcm%s", _active->library().latin1());
	  dummyAbout = new KAboutData(buffer,
				      _active->name(), "2.0");
	  deleteit = true;
	}
    }
    KBugReport *br = new KBugReport(this, false, dummyAbout);
    if (deleteit) {
	connect(br, SIGNAL(finished()), SLOT(deleteDummyAbout()));
    } else
      dummyAbout = 0;
    br->show();
}
