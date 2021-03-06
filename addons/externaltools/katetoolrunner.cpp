/* This file is part of the KDE project
 *
 *  Copyright 2019 Dominik Haumann <dhaumann@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "katetoolrunner.h"

#include "kateexternaltool.h"

#include <KLocalizedString>
#include <KShell>
#include <KTextEditor/View>
#include <QFileInfo>

KateToolRunner::KateToolRunner(std::unique_ptr<KateExternalTool> tool, KTextEditor::View *view, QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_tool(std::move(tool))
    , m_process(new QProcess())
{
    m_process->setProcessChannelMode(QProcess::SeparateChannels);
}

KateToolRunner::~KateToolRunner()
{
}

KTextEditor::View *KateToolRunner::view() const
{
    return m_view;
}

KateExternalTool *KateToolRunner::tool() const
{
    return m_tool.get();
}

void KateToolRunner::run()
{
    if (!m_tool->workingDir.isEmpty()) {
        m_process->setWorkingDirectory(m_tool->workingDir);
    } else if (m_view) {
        // if nothing is set, use the current document's directory
        const auto url = m_view->document()->url();
        if (url.isLocalFile()) {
            const QString localFilePath = url.toLocalFile();
            m_process->setWorkingDirectory(QFileInfo(localFilePath).absolutePath());
        }
    }

    QObject::connect(m_process.get(), &QProcess::readyReadStandardOutput, [this]() { m_stdout += m_process->readAllStandardOutput(); });
    QObject::connect(m_process.get(), &QProcess::readyReadStandardError, [this]() { m_stderr += m_process->readAllStandardError(); });
    QObject::connect(
        m_process.get(), static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), [this](int exitCode, QProcess::ExitStatus exitStatus) { Q_EMIT toolFinished(this, exitCode, exitStatus == QProcess::CrashExit); });

    // Write stdin to process, if applicable, then close write channel
    QObject::connect(m_process.get(), &QProcess::started, [this]() {
        if (!m_tool->input.isEmpty()) {
            m_process->write(m_tool->input.toLocal8Bit());
        }
        m_process->closeWriteChannel();
    });

    const QStringList args = KShell::splitArgs(m_tool->arguments);
    m_process->start(m_tool->executable, args);
}

void KateToolRunner::waitForFinished()
{
    m_process->waitForFinished();
}

QString KateToolRunner::outputData() const
{
    return QString::fromLocal8Bit(m_stdout);
}

QString KateToolRunner::errorData() const
{
    return QString::fromLocal8Bit(m_stderr);
}

// kate: space-indent on; indent-width 4; replace-tabs on;
