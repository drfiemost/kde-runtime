/*
 * This file was generated by dbusxml2cpp version 0.6
 * Command line was: dbusxml2cpp -l UIServer -m -i uiserver.h -a uiserveradaptor_p.h:uiserveradaptor.cpp -- org.kde.KIO.UIServer.xml
 *
 * dbusxml2cpp is Copyright (C) 2006 Trolltech AS. All rights reserved.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "uiserveradaptor_p.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class UIServerAdaptor
 */

UIServerAdaptor::UIServerAdaptor(UIServer *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

UIServerAdaptor::~UIServerAdaptor()
{
    // destructor
}

void UIServerAdaptor::canResume(int id, qulonglong offset)
{
    // handle method call org.kde.KIO.UIServer.canResume
    parent()->canResume(id, offset);
}

void UIServerAdaptor::copying(int id, const QString &from, const QString &to)
{
    // handle method call org.kde.KIO.UIServer.copying
    parent()->copying(id, from, to);
}

void UIServerAdaptor::creatingDir(int id, const QString &url)
{
    // handle method call org.kde.KIO.UIServer.creatingDir
    parent()->creatingDir(id, url);
}

void UIServerAdaptor::deleting(int id, const QString &url)
{
    // handle method call org.kde.KIO.UIServer.deleting
    parent()->deleting(id, url);
}

void UIServerAdaptor::infoMessage(int id, const QString &msg)
{
    // handle method call org.kde.KIO.UIServer.infoMessage
    parent()->infoMessage(id, msg);
}

void UIServerAdaptor::jobFinished(int id)
{
    // handle method call org.kde.KIO.UIServer.jobFinished
    parent()->jobFinished(id);
}

int UIServerAdaptor::messageBox(int id, int type, const QString &text, const QString &caption, const QString &buttonYes, const QString &buttonNo)
{
    // handle method call org.kde.KIO.UIServer.messageBox
    return parent()->messageBox(id, type, text, caption, buttonYes, buttonNo);
}

void UIServerAdaptor::mounting(int id, const QString &dev, const QString &point)
{
    // handle method call org.kde.KIO.UIServer.mounting
    parent()->mounting(id, dev, point);
}

void UIServerAdaptor::moving(int id, const QString &from, const QString &to)
{
    // handle method call org.kde.KIO.UIServer.moving
    parent()->moving(id, from, to);
}

int UIServerAdaptor::newJob(const QString &appServiceName, bool showProgress, const QString &internalAppName, const QString &jobIcon, const QString &appName)
{
    // handle method call org.kde.KIO.UIServer.newJob
    return parent()->newJob(appServiceName, showProgress, internalAppName, jobIcon, appName);
}

int UIServerAdaptor::newAction(int jobId, const QString &actionText)
{
    return parent()->newAction(jobId, actionText);
}

void UIServerAdaptor::percent(int id, uint ipercent)
{
    // handle method call org.kde.KIO.UIServer.percent
    parent()->percent(id, ipercent);
}

void UIServerAdaptor::processedDirs(int id, uint dirs)
{
    // handle method call org.kde.KIO.UIServer.processedDirs
    parent()->processedDirs(id, dirs);
}

void UIServerAdaptor::processedFiles(int id, uint files)
{
    // handle method call org.kde.KIO.UIServer.processedFiles
    parent()->processedFiles(id, files);
}

void UIServerAdaptor::processedSize(int id, qulonglong size)
{
    // handle method call org.kde.KIO.UIServer.processedSize
    parent()->processedSize(id, size);
}

void UIServerAdaptor::setJobVisible(int id, bool enable)
{
    // handle method call org.kde.KIO.UIServer.setJobVisible
    parent()->setJobVisible(id, enable);
}

bool UIServerAdaptor::showSSLCertDialog(const QString &host, const QStringList &certList, qlonglong mainwindow, bool &send, bool &save, QString &choice)
{
    // handle method call org.kde.KIO.UIServer.showSSLCertDialog
    // HAND EDIT: expand the return values
    KSSLCertDialogRet ret = parent()->showSSLCertDialog(host, certList, mainwindow);
    save = ret.save;
    choice = ret.choice;
    send = ret.send;
    return ret.ok;
}

void UIServerAdaptor::showSSLInfoDialog(const QString &url, const QMap<QString,QString> &data, qlonglong mainwindow)
{
    // handle method call org.kde.KIO.UIServer.showSSLInfoDialog
    parent()->showSSLInfoDialog(url, data, mainwindow);
}

void UIServerAdaptor::speed(int id, uint bytesPerSecond)
{
    // handle method call org.kde.KIO.UIServer.speed
    parent()->speed(id, bytesPerSecond);
}

void UIServerAdaptor::stating(int id, const QString &url)
{
    // handle method call org.kde.KIO.UIServer.stating
    parent()->stating(id, url);
}

void UIServerAdaptor::totalDirs(int id, uint dirs)
{
    // handle method call org.kde.KIO.UIServer.totalDirs
    parent()->totalDirs(id, dirs);
}

void UIServerAdaptor::totalFiles(int id, uint files)
{
    // handle method call org.kde.KIO.UIServer.totalFiles
    parent()->totalFiles(id, files);
}

void UIServerAdaptor::totalSize(int id, qulonglong size)
{
    // handle method call org.kde.KIO.UIServer.totalSize
    parent()->totalSize(id, size);
}

void UIServerAdaptor::transferring(int id, const QString &url)
{
    // handle method call org.kde.KIO.UIServer.transferring
    parent()->transferring(id, url);
}

void UIServerAdaptor::unmounting(int id, const QString &point)
{
    // handle method call org.kde.KIO.UIServer.unmounting
    parent()->unmounting(id, point);
}


#include "uiserveradaptor_p.moc"
