/* This file is part of the KDE libraries
   Copyright (C) 2004 George Staikos <staikos@kde.org>

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
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef KWALLETWIZARD_H
#define KWALLETWIZARD_H

#include <QWizard>
#ifdef HAVE_GPGMEPP
#include <gpgme++/key.h>
#endif

class PageGpgKey;
class PagePassword;
class PageIntro;

class KWalletWizard : public QWizard
{
  Q_OBJECT
  public:

        enum WizardType
        {
            Basic,
            Advanced
        };

        static const int PageIntroId = 0;
        static const int PagePasswordId = 1;
#ifdef HAVE_GPGMEPP
        static const int PageGpgKeyId =2;
#endif
        static const int PageOptionsId = 3;
        static const int PageExplanationId = 4;

    KWalletWizard( QWidget *parent = 0 );

        WizardType wizardType() const;

#ifdef HAVE_GPGMEPP
        GpgME::Key gpgKey() const;
#endif // HAVE_GPGMEPP

    protected:
        virtual void initializePage(int id);

  protected Q_SLOTS:
    void passwordPageUpdate();

    private:
        PageIntro *m_pageIntro;
        PagePassword *m_pagePasswd;
#ifdef HAVE_GPGMEPP
        PageGpgKey *m_pageGpgKey;
#endif
};

#endif
