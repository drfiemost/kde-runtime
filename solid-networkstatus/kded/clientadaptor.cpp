/*
 * This file was generated by dbusidl2cpp version 0.5
 * when processing input file org.kde.networkstatus.client.xml
 *
 * dbusidl2cpp is Copyright (C) 2006 Trolltech AS. All rights reserved.
 *
 * This is an auto-generated file.
 */

#include "clientadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class ClientAdaptor
 */

ClientAdaptor::ClientAdaptor(QObject *parent)
   : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

ClientAdaptor::~ClientAdaptor()
{
    // destructor
}

QStringList ClientAdaptor::networks()
{
    // handle method call org.kde.networkstatus.client.networks
    QStringList out0;
    QMetaObject::invokeMethod(parent(), "networks", Q_RETURN_ARG(QStringList, out0));

    // Alternative:
    //out0 = static_cast<YourObjectType *>(parent())->networks();
    return out0;
}

void ClientAdaptor::relinquish(const QString &host)
{
    // handle method call org.kde.networkstatus.client.relinquish
    QMetaObject::invokeMethod(parent(), "relinquish", Q_ARG(QString, host));

    // Alternative:
    //static_cast<YourObjectType *>(parent())->relinquish(host);
}

bool ClientAdaptor::reportFailure(const QString &host)
{
    // handle method call org.kde.networkstatus.client.reportFailure
    bool out0;
    QMetaObject::invokeMethod(parent(), "reportFailure", Q_RETURN_ARG(bool, out0), Q_ARG(QString, host));

    // Alternative:
    //out0 = static_cast<YourObjectType *>(parent())->reportFailure(host);
    return out0;
}

int ClientAdaptor::request(const QString &host, bool userInitiated)
{
    // handle method call org.kde.networkstatus.client.request
    int out0;
    QMetaObject::invokeMethod(parent(), "request", Q_RETURN_ARG(int, out0), Q_ARG(QString, host), Q_ARG(bool, userInitiated));

    // Alternative:
    //out0 = static_cast<YourObjectType *>(parent())->request(host, userInitiated);
    return out0;
}

int ClientAdaptor::status(const QString &host)
{
    // handle method call org.kde.networkstatus.client.status
    int out0;
    QMetaObject::invokeMethod(parent(), "status", Q_RETURN_ARG(int, out0), Q_ARG(QString, host));

    // Alternative:
    //out0 = static_cast<YourObjectType *>(parent())->status(host);
    return out0;
}


#include "clientadaptor.moc"
