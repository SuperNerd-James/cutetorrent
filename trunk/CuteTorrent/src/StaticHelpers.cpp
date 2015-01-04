/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "CuteTorrent.h"
#include "StaticHelpers.h"
#include <QDebug>


QString StaticHelpers::toKbMbGb(libtorrent::size_type size)
{
    float val = size;
    char* Suffix[] = { " B\0", " KB\0", " MB\0", " GB\0", " TB\0", " PB\0", " EB\0" , " ZB\0" };
    int i = 0;
    float dblSByte = val;

    if(size > 1024)
        for(i ; (libtorrent::size_type)(val / 1024) > 0; i++, val /= 1024)
        {
            dblSByte = val / 1024.f;
        }

    QString str = QString::number(dblSByte, 'f', i == 0 ? 0 : 2);
    str.append(Suffix[i]);
    return str;
}

void StaticHelpers::dellDir(QString dirName)
{
    try
    {
        error_code ec;
        remove_all(dirName.toStdString(), ec);
    }
    catch(...)
    {
        qDebug() << " exception caught int deldir";
    }
}
QString StaticHelpers::filePriorityToString(int priority)
{
    static char* priority_str[] = {"FILETAB_PRIORITY_ZERO", "FILETAB_PRIORITY_LOW", "FILETAB_PRIORITY_MEDIUM", "FILETAB_PRIORITY_HIGH"};

    if(priority > 0)
    {
        if(priority == 0)
        {
            return CuteTorrent::tr(priority_str[0]);
        }

        if(priority < 3)
        {
            return CuteTorrent::tr(priority_str[1]);
        }

        if(priority >= 3 && priority < 6)
        {
            return CuteTorrent::tr(priority_str[2]);
        }

        if(priority >= 6)
        {
            return CuteTorrent::tr(priority_str[3]);
        }
    }

    return "";
}

QString StaticHelpers::toTimeString(int seconds)
{
    if(seconds < 0)
    {
        return "";
    }

    int min = 0, hour = 0, day = 0, Week = 0;
    Week = seconds / (7 * 24 * 60 * 60);
    seconds -= Week * (7 * 24 * 60 * 60);
    day = seconds / (24 * 60 * 60);
    seconds -= day * (24 * 60 * 60);
    hour = seconds / (60 * 60);
    seconds -= hour * 60 * 60;
    min = seconds / 60;
    seconds -= min * 60;
    QString result, tmp;

    if(Week > 0)
    {
        result.append(QObject::tr("%1 WEEK ").arg(Week));
    }

    if(day > 0)
    {
        result.append(QObject::tr("%1 DAY ").arg(day));
    }

    if(hour >= 0)
    {
        result.append(tmp.sprintf("%02d:", hour));
    }

    if(min >= 0)
    {
        result.append(tmp.sprintf("%02d:", min));
    }

    result.append(tmp.sprintf("%02d", seconds));
    return result;
}

QString StaticHelpers::SchedulerTypeToString(SchedulerTask::TaskType type)
{
    switch(type)
    {
        case SchedulerTask::START_ALL :
            return "START_ALL";

        case SchedulerTask::PAUSE_ALL:
            return "PAUSE_ALL";

        case SchedulerTask::LIMIT_UPLOAD:
            return "LIMIT_UL";

        case SchedulerTask::LIMIT_DOWNLOAD:
            return "LIMIT_DL";
    }

    return "";
}

QString StaticHelpers::GetBaseSuffix(const libtorrent::file_storage& storrage)
{
    QString base_suffix;
    int maxSuffix = 0;
    QMap<QString, int> suffixesCount;
    libtorrent::file_storage::iterator iter = storrage.begin();

    for(iter; iter != storrage.end(); ++iter)
    {
        QFileInfo curfile(QString::fromUtf8(storrage.file_path(*iter).c_str()));

        if(StyleEngene::suffixes[StyleEngene::DISK].contains(curfile.suffix()))
        {
            base_suffix = curfile.suffix();
            break;
        }

        if(StyleEngene::suffixes[StyleEngene::VIDEO].contains(curfile.suffix()))
        {
            base_suffix = curfile.suffix();
            break;
        }

        if(!suffixesCount.contains(curfile.suffix()))
        {
            suffixesCount.insert(curfile.suffix(), 1);
        }
        else
        {
            suffixesCount[curfile.suffix()]++;
        }

        if(suffixesCount[curfile.suffix()] > maxSuffix)
        {
            maxSuffix = suffixesCount[curfile.suffix()];
            base_suffix = curfile.suffix();
        }
    }

    return base_suffix;
}


