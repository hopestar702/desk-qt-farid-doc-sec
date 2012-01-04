/***************************************************************************
 *   Copyright (C) 2011 by Francesco Cecconi                               *
 *   francesco.cecconi@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "logwriter.h"

logWriter::logWriter()
{
}

void logWriter::writeAllLogFile(QList<parserObj*> pObjectList, const QString& path)
{
    QSettings settings("nmapsi4", "nmapsi4");
    int logType = settings.value("logType", 0).toInt();

    foreach (parserObj* object, pObjectList)
    {
        if (object->isValidObject())
        {
            QString pathTmp(path);
            if (!pathTmp.endsWith(QDir::toNativeSeparators("/")))
            {
                pathTmp.append(QDir::toNativeSeparators("/"));
            }

            QString fileName(object->getHostName());
            fileName = fileName.replace('.','_');

            if (logType == HtmlLog)
            {
                fileName.append(".html");
            }
            else
            {
                fileName.append(".log");
            }

            pathTmp.append(fileName);

            writeSingleLogFile(object, pathTmp);
        }
    }
}

void logWriter::writeSingleLogFile(parserObj* pObject, const QString& path)
{
    m_pObject = pObject;

    // check log type and call the private method
    QSettings settings("nmapsi4", "nmapsi4");

    int logType = settings.value("logType", 0).toInt();

    if (path.endsWith(".html"))
    {
        // force html log format from file extension
        logType = HtmlLog;
    }

    switch (logType)
    {
    case FancyLog:
        writeFancyLogFormat(path);
        break;
    case RawLog:
        writeRawLogFormat(path);
        break;
    case HtmlLog:
        writeHtmlLogFormat(path);
        break;
    }

}

void logWriter::writeFancyLogFormat(const QString& path)
{
    QFile *filePtr = new QFile(path);

    if (!filePtr->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "DEBUG::File Writer:: Problem with file open";
        return;
    }

    QTextStream fileStream(filePtr);

    // hostname
    fileStream << "\n" << m_pObject->getParameters() << "\n\n";
    fileStream << "|---------- Services" << "\n\n";

    // Open Ports
    foreach (const QString &token, m_pObject->getPortOpen())
    {
        fileStream << token << "\n";
    }

    // close Ports
    foreach (const QString &token, m_pObject->getPortClose())
    {
        fileStream << token << "\n";
    }

    // filtered/unfilteres Ports
    foreach (const QString &token, m_pObject->getPortFiltered())
    {
        fileStream << token << "\n";
    }

    fileStream << "\n|---------- General information" << "\n\n";

    // filtered/unfilteres Ports
    foreach (const QString &token, m_pObject->getMainInfo())
    {
        fileStream << token << "\n";
    }

    fileStream << "\n|---------- Nse result" << "\n";

    // Show Nss Info
    QHash<QString, QStringList> nseResult = m_pObject->getNseResult();
    QHash<QString, QStringList>::const_iterator i;

    for (i = nseResult.constBegin(); i != nseResult.constEnd(); ++i)
    {
        fileStream << "\n--- " << i.key() << "\n\n";

        foreach (const QString& value, i.value())
        {
            fileStream << value << "\n";
        }
    }

    fileStream << "\n|---------- Scan Errors/Warning" << "\n\n";

    // scan errors/Warning
    foreach (const QString &token, m_pObject->getErrorScan())
    {
        fileStream << token << "\n";
    }

    filePtr->close();
    delete filePtr;
}

void logWriter::writeRawLogFormat(const QString& path)
{
    QFile *filePtr = new QFile(path);

    if (!filePtr->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "DEBUG::File Writer:: Problem with file open";
        return;
    }

    QTextStream fileStream(filePtr);

    foreach (const QString& token, m_pObject->getFullScanLog())
    {
        fileStream << token << "\n";
    }

    filePtr->close();
    delete filePtr;
}

void logWriter::writeHtmlLogFormat(const QString& path)
{
    QFile *filePtr = new QFile(path);

    if (!filePtr->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "DEBUG::File Writer:: Problem with file open";
        return;
    }

    QStringList scanValues = m_pObject->getHostName().split(' ', QString::SkipEmptyParts);
    const QString& hostName = scanValues[scanValues.size()-1];

    QString htmlPage;
    // Html header
    htmlPage += "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\"\"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">";
    htmlPage += "<html xmlns=\"http://www.w3.org/1999/xhtml\">";
    htmlPage += "<head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />";
    htmlPage += "<title>" + hostName + "</title>";
    // css Style
    htmlPage += "<style type=\"text/css\">";
    htmlPage += ".head { height:20px; width:695px; background: #ccc; color: #000; float: left; padding: 5px; }";
    htmlPage += ".sectionHead { height:20px; width:695px; background: #ccc; color: #000; float: left; padding: 5px; }";
    htmlPage += ".container { width:700px; background: #82b9ed; color: #000; float: left; padding: 2px; }";
    htmlPage += ".title { height:20px; width:690px; background: #82b9ed; color: #000; float: left; padding: 5px; }";
    htmlPage += ".result { width:690px; background: #fff; color: #000; float: left; padding: 5px; }";
    htmlPage += ".space { height: 20px; width:700px; background: #fff; float: left; }";
    htmlPage += "</style>";
    //Html core
    htmlPage += "</style></head><body>";
    // hostname
    htmlPage += "<div class=\"head\"><b>Scan parameters:</b> ";
    htmlPage += m_pObject->getParameters();
    htmlPage += "</div>";

    //htmlPage += "<div class=\"space\">&nbsp;</div>";

    // Open Ports
    htmlPage += "<div class=\"container\"><div class=\"title\"><b>Services</b></div>";
    htmlPage += "<div class=\"result\">";
    foreach (const QString &token, m_pObject->getPortOpen())
    {
        htmlPage += token +"<br/>\n";
    }

    // close Ports
    foreach (const QString &token, m_pObject->getPortClose())
    {
        htmlPage += token +"<br/>\n";
    }

    // filtered/unfilteres Ports
    foreach (const QString &token, m_pObject->getPortFiltered())
    {
        htmlPage += token +"<br/>\n";
    }

    htmlPage += "</div></div>";
    //htmlPage += "<div class=\"space\">&nbsp;</div>";

    // Info
    htmlPage += "<div class=\"container\"><div class=\"title\"><b>General information</b></div>";
    htmlPage += "<div class=\"result\">";
    foreach (const QString &token, m_pObject->getMainInfo())
    {
        htmlPage +=token +"<br/>\n";
    }
    htmlPage += "</div></div>";
    //htmlPage += "<div class=\"space\">&nbsp;</div>";

    // scan errors/Warning
    htmlPage += "<div class=\"container\"><div class=\"title\"><b>Scan Errors/Warning</b></div>";
    htmlPage += "<div class=\"result\">";
    foreach (const QString &token, m_pObject->getErrorScan())
    {
        htmlPage += token + "<br/>\n";
    }
    htmlPage += "</div></div>";
    //htmlPage += "<div class=\"space\">&nbsp;</div>";

    // Show Nss Info
    htmlPage += "<div class=\"sectionHead\"><b>Nse result</b></div>";
    //htmlPage += "<div class=\"space\">&nbsp;</div>";
    QHash<QString, QStringList> nseResult = m_pObject->getNseResult();
    QHash<QString, QStringList>::const_iterator i;

    for (i = nseResult.constBegin(); i != nseResult.constEnd(); ++i)
    {
        htmlPage += "<div class=\"container\"><div class=\"title\">";
        htmlPage += "<b>" + i.key() + "</b>\n";
        htmlPage += "</div>";

        htmlPage += "<div class=\"result\">";
        foreach (const QString& value, i.value())
        {
            htmlPage += value + "<br/>\n";
        }

        htmlPage += "</div></div>";
    }
    htmlPage += "<div class=\"space\">&nbsp;</div>";
    htmlPage += "</body></html>";

    // write html page into file
    QTextStream fileStream(filePtr);
    fileStream << htmlPage;

    filePtr->close();
    delete filePtr;
}
