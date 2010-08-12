/*
 *   Copyright 2007-2008 Richard J. Moore <rich@kde.org>
 *   Copyright 2009 Aaron J. Seigo <aseigo@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License version 2 as
 *   published by the Free Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "scriptenv.h"

#include <iostream>

#include <QFile>
#include <QMetaEnum>

#include <KDebug>
#include <KIO/Job>
#include <KLocale>
#include <KMimeType>
#include <KService>
#include <KShell>
#include <KStandardDirs>
#include <KRun>

#ifdef USEGUI
#include "simplebindings/filedialogproxy.h"
#endif

Q_DECLARE_METATYPE(ScriptEnv*)

ScriptEnv::ScriptEnv(QObject *parent, QScriptEngine *engine)
    : QObject(parent),
      m_allowedUrls(NoUrls),
      m_engine(engine)
{
    QScriptValue global = m_engine->globalObject();

    // Add utility functions
    global.setProperty("print", m_engine->newFunction(ScriptEnv::print));
    global.setProperty("debug", m_engine->newFunction(ScriptEnv::debug));

    // Add an accessor so we can find the scriptenv given only the engine. The
    // property is hidden from scripts.
    global.setProperty("__plasma_scriptenv", m_engine->newQObject(this),
                       QScriptValue::ReadOnly|QScriptValue::Undeletable|QScriptValue::SkipInEnumeration);
    connect(m_engine, SIGNAL(signalHandlerException(QScriptValue)), this, SLOT(signalException()));
}

ScriptEnv::~ScriptEnv()
{
}

QScriptEngine *ScriptEnv::engine() const
{
    return m_engine;
}

ScriptEnv *ScriptEnv::findScriptEnv(QScriptEngine *engine)
{
    QScriptValue global = engine->globalObject();
    return qscriptvalue_cast<ScriptEnv*>(global.property("__plasma_scriptenv"));
}

void ScriptEnv::signalException()
{
    checkForErrors(false);
}

void ScriptEnv::registerEnums(QScriptValue &scriptValue, const QMetaObject &meta)
{
    //manually create enum values. ugh
    QScriptEngine *engine = scriptValue.engine();
    for (int i = 0; i < meta.enumeratorCount(); ++i) {
        QMetaEnum e = meta.enumerator(i);
        //kDebug() << e.name();
        for (int i=0; i < e.keyCount(); ++i) {
            //kDebug() << e.key(i) << e.value(i);
            scriptValue.setProperty(e.key(i), QScriptValue(engine, e.value(i)));
        }
    }
}

bool ScriptEnv::include(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        kWarning() << i18n("Unable to load script file: %1", path);
        return false;
    }

    QString script = file.readAll();
    //kDebug() << "Script says" << script;

    // change the context to the parent context so that the include is actually
    // executed in the same context as the caller; seems to be what javascript
    // coders expect :)
    QScriptContext *ctx = m_engine->currentContext();
    if (ctx && ctx->parentContext()) {
        ctx->setActivationObject(ctx->parentContext()->activationObject());
        ctx->setThisObject(ctx->parentContext()->thisObject());
    }

    m_engine->evaluate(script, path);

    return !checkForErrors(true);
}

bool ScriptEnv::checkForErrors(bool fatal)
{
    if (m_engine->hasUncaughtException()) {
        emit reportError(this, fatal);
        if (!fatal) {
            m_engine->clearExceptions();
        }
        return true;
    }

    return false;
}

QScriptValue ScriptEnv::runApplication(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine)
    if (context->argumentCount() == 0) {
        return false;
    }

    KUrl::List urls;
    if (context->argumentCount() > 1) {
        urls = qscriptvalue_cast<KUrl::List>(context->argument(1));
    }

    const QString app = context->argument(0).toString();

    const QString exec = KGlobal::dirs()->findExe(app);
    if (!exec.isEmpty()) {
        return KRun::run(exec, urls, 0);
    }

    KService::Ptr service = KService::serviceByStorageId(app);
    if (service) {
        return KRun::run(*service, urls, 0);
    }

    return false;
}

QScriptValue ScriptEnv::runCommand(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine);

    if (context->argumentCount() == 0) {
        return false;
    }

    const QString exec = KGlobal::dirs()->findExe(context->argument(0).toString());
    if (!exec.isEmpty()) {
        QString args;
        if (context->argumentCount() > 1) {
            const QStringList argList = qscriptvalue_cast<QStringList>(context->argument(1));
            if (!argList.isEmpty()) {
                args = ' ' + KShell::joinArgs(argList);
            }
        }

        return KRun::runCommand(exec + args, 0);
    }

    return false;
}

QScriptValue ScriptEnv::openUrl(QScriptContext *context, QScriptEngine *engine)
{
    Q_UNUSED(engine)
    if (context->argumentCount() == 0) {
        return false;
    }

    QScriptValue v = context->argument(0);
    KUrl url = v.isString() ? KUrl(v.toString()) : qscriptvalue_cast<KUrl>(v);
    if (url.isValid()) {
        return KRun::runUrl(url, KMimeType::findByUrl(url)->name(), 0);
    }

    return false;
}

// TODO these should throw an exception
QScriptValue ScriptEnv::getUrl(QScriptContext *context, QScriptEngine *engine)
{
    if (context->argumentCount() == 0) {
        return engine->undefinedValue();
    }

    QScriptValue v = context->argument(0);
    KUrl url = v.isString() ? KUrl(v.toString()) : qscriptvalue_cast<KUrl>(v);

    if (!url.isValid()) {
        return engine->undefinedValue();
    }

    ScriptEnv *env = ScriptEnv::findScriptEnv(engine);
    if (!env) {
	kDebug() << "findScriptEnv failed";
        return engine->undefinedValue();
    }

    if (url.isLocalFile()) {
        if (!(env->m_allowedUrls & LocalUrls)) {
            return engine->undefinedValue();
        }
    } else if (!(env->m_allowedUrls & NetworkUrls) &&
               !((env->m_allowedUrls & HttpUrls) && (url.protocol() == "http" || url.protocol() == "https"))) {
        return engine->undefinedValue();
    }

    KIO::Job *job = KIO::get(url, KIO::NoReload, KIO::HideProgressInfo);
    return engine->newQObject(job);
}

void ScriptEnv::registerGetUrl(QScriptValue &obj)
{
    QScriptValue get = obj.property("getUrl");
    if (!get.isValid()) {
        obj.setProperty("getUrl", m_engine->newFunction(ScriptEnv::getUrl));
    }
}

bool ScriptEnv::importBuiltinExtension(const QString &extension, QScriptValue &obj)
{
    kDebug() << extension;
    if ("filedialog" == extension) {
#ifdef USEGUI
        FileDialogProxy::registerWithRuntime(m_engine);
        return true;
#endif
    } else if ("launchapp" == extension) {
        obj.setProperty("runApplication", m_engine->newFunction(ScriptEnv::runApplication));
        obj.setProperty("runCommand", m_engine->newFunction(ScriptEnv::runCommand));
        obj.setProperty("openUrl", m_engine->newFunction(ScriptEnv::openUrl));
        return true;
    } else if ("http" == extension) {
        m_allowedUrls |= HttpUrls;
        registerGetUrl(obj);
        return true;
    } else if ("networkio" == extension) {
        m_allowedUrls |= HttpUrls | NetworkUrls;
        registerGetUrl(obj);
        return true;
    } else if ("localio" == extension) {
        m_allowedUrls |= LocalUrls;
        registerGetUrl(obj);
        return true;
    }

    return false;
}

bool ScriptEnv::importExtensions(const KPluginInfo &info, QScriptValue &obj, Authorization &auth)
{
    QStringList requiredExtensions = info.service()->property("X-Plasma-RequiredExtensions", QVariant::StringList).toStringList();
    kDebug() << "required extensions are" << requiredExtensions;
    foreach (const QString &ext, requiredExtensions) {
        QString extension = ext.toLower();
        if (m_extensions.contains(extension)) {
            continue;
        }

        if (!auth.authorizeRequiredExtension(extension)) {
            return false;
        }

        if (!importBuiltinExtension(extension, obj)) {
            if (auth.authorizeExternalExtensions()) {
                m_engine->importExtension(extension);
            }
        }

        if (checkForErrors(true)) {
            return false;
        } else {
            m_extensions << extension;
        }
    }

    QStringList optionalExtensions = info.service()->property("X-Plasma-OptionalExtensions", QVariant::StringList).toStringList();
    kDebug() << "optional extensions are" << optionalExtensions;
    foreach (const QString &ext, optionalExtensions) {
        QString extension = ext.toLower();

        if (m_extensions.contains(extension)) {
            continue;
        }

        if (!auth.authorizeOptionalExtension(extension)) {
            continue;
        }

        if (!importBuiltinExtension(extension, obj)) {
            if (auth.authorizeExternalExtensions()) {
                m_engine->importExtension(extension);
            }
        }

        if (!checkForErrors(false)) {
            m_extensions << extension;
        }
    }

    return true;
}

QSet<QString> ScriptEnv::loadedExtensions() const
{
    return m_extensions;
}

QScriptValue ScriptEnv::debug(QScriptContext *context, QScriptEngine *engine)
{
    if (context->argumentCount() != 1) {
        return context->throwError(i18n("debug takes one argument"));
    }

    kDebug() << context->argument(0).toString();
    return engine->undefinedValue();
}

QScriptValue ScriptEnv::print(QScriptContext *context, QScriptEngine *engine)
{
    if (context->argumentCount() != 1) {
        return context->throwError(i18n("print() takes one argument"));
    }

    std::cout << context->argument(0).toString().toLocal8Bit().constData() << std::endl;
    return engine->undefinedValue();
}

#ifndef USEGUI
#include "scriptenv.moc"
#endif