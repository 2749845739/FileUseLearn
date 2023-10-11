/** Copyright (C) Fuqing Medical and USTC BMEC RFLab - All Rights Reserved.
 ** Unauthorized copying of this file, via any medium is strictly prohibited.
 ** Proprietary and confidential.
 ** Created on 20230706, by yue.wang.
 **/
#include "backupfile.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QByteArray>
#include <QCoreApplication>
#include "LabTool.h"
#include <QStandardPaths>

BackupFile::BackupFile(QObject *parent) : QObject(parent), m_fileInfos()
{
}

int BackupFile::getBackupCycle()
{
    int cycle;
    QJsonObject systemConfig = LabTool::ReadJsonFile("config/sysInfo.json");
    if(systemConfig.contains("backupCycle")){
        QJsonValue cycleValue = systemConfig.value("backupCycle");
        cycle = cycleValue.toInt();
    }else{
        qWarning() << "sysInfo.json: backupCycle not exist";
    }
    return cycle;
}

int BackupFile::clearFolder(const QString &folderPath, const int &uplimit, const int &lowlimit)
{
    QDir folder(folderPath);
    if(!folder.exists()){
        qWarning() << "ClearFolder failed, folder path does not exist: " << folderPath;
        return -1;
    }

    QFileInfoList fileList = folder.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
    int fileCount = fileList.count();
    if(fileCount <= uplimit){
        qDebug() << "ClearFolder: No need to clean the folder.";
        return 0;
    }

    for(int i = lowlimit; i < fileCount; ++i){
        QString filePath = fileList.at(i).absoluteFilePath();
        QDir delFolder(filePath);
        bool removed = delFolder.removeRecursively();
        qDebug() << "Removed" << fileList.at(i).fileName() << (removed ? "Success" : "Failed");
    }
    return 0;
}

int BackupFile::compressFolder(const QString &sourceFolder, const QString &destinationFolder, const QString &zipFileName)
{
    QDir sourceFolderDir(sourceFolder);
    if(!sourceFolderDir.exists()){
        qWarning() << "Compress failed, folder does not exist：" << sourceFolder;
        return -1;
    }

    QStringList depends;
    depends << "7z.exe" << "7z.dll";
    for(const QString &file : depends){
        QString strFile = QCoreApplication::applicationDirPath() + "/" + file;
        if (!QFile::exists(strFile)){
            qWarning() << "Compress failed, missing dependency library" << strFile;
            return -2;
        }
    }

    QString zipFilePath = destinationFolder + "/" + zipFileName;
    QStringList arguments;
    arguments << "a"<< zipFilePath << sourceFolder;
    QProcess process;
    process.start(QCoreApplication::applicationDirPath() + "/7z.exe", arguments);
    process.waitForFinished();
    return 0;
}

int BackupFile::decompressFolder(const QString &filePath, const QString &destinationPath)
{
    if(!QFile::exists(filePath)){
        qWarning() << "Decompress failed, the compressed package does not exist" << filePath;
        return -1;
    }
    QDir desFolderDir(destinationPath);
    if(!desFolderDir.exists()){
        qWarning() << "Decompress failed, path does not exist：" << destinationPath;
        return -2;
    }

    QStringList depends;
    depends << "7z.exe" << "7z.dll";
    for(const QString &file : depends){
        QString strFile = QCoreApplication::applicationDirPath() + "/" + file;
        if (!QFile::exists(strFile)){
            qWarning() << "Decompress failed, missing dependency library" << strFile;
            return -3;
        }
    }

    QStringList arguments;
    arguments << "x" << "-o" + destinationPath << filePath << "-r" <<"-aoa";
    QProcess process;
    QString program = QCoreApplication::applicationDirPath() + "/7z.exe";
    process.start(program, arguments);
    process.waitForFinished();
    return 0;
}


bool BackupFile::mergeFileInfos(const QString &folderPath1, const QString &folderPath2)
{
    QDir folderDir1(folderPath1);
    QDir folderDir2(folderPath2);
    if(!folderDir1.exists() || !folderDir2.exists()){
        qWarning() << "MergeFileInfos failed, folder does not exist" << folderPath1 << ", " << folderPath2;
        return false;
    }

    m_fileInfos.clear();
    QFileInfoList fileInfoList1 = folderDir1.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList2 = folderDir2.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach(QFileInfo fileInfo, fileInfoList1){
        FileInfo fileInfoStruct;
        fileInfoStruct.fileName = fileInfo.fileName();
        fileInfoStruct.birthTime = fileInfo.birthTime().toString(DATEFORMAT);
        fileInfoStruct.path = fileInfo.absoluteFilePath();
        fileInfoStruct.source = 1;
        m_fileInfos.push_back(fileInfoStruct);
    }
    foreach(QFileInfo fileInfo, fileInfoList2){
        FileInfo fileInfoStruct;
        fileInfoStruct.fileName = fileInfo.fileName();
        fileInfoStruct.birthTime = fileInfo.birthTime().toString(DATEFORMAT);
        fileInfoStruct.path = fileInfo.absoluteFilePath();
        fileInfoStruct.source = 2;
        m_fileInfos.push_back(fileInfoStruct);
    }

    std::sort(m_fileInfos.begin(), m_fileInfos.end(), [](const FileInfo& fileInfo1, const FileInfo& fileInfo2) {
        return fileInfo1.birthTime > fileInfo2.birthTime;
    });

    return true;
}

QStringList BackupFile::getFileTimes()
{
    QStringList fileTimeList;
    foreach(FileInfo fileInfo, m_fileInfos){
        fileTimeList.push_back(fileInfo.birthTime);
    }
    return fileTimeList;
}

QStringList BackupFile::getFileSources()
{
    QStringList fileSourceList;
    foreach(FileInfo fileInfo, m_fileInfos){
        fileSourceList.push_back(fileInfo.source == 1 ? "Fast" : "Scheduled");
    }
    return fileSourceList;
}

QString BackupFile::getFilePath(QString time)
{
    auto it = std::find_if(m_fileInfos.begin(), m_fileInfos.end(), [&](const FileInfo& fileInfo) {
        return fileInfo.birthTime == time;
    });
    QString resultPath;
    if(it != m_fileInfos.end()){
        resultPath = it->path;
    }else{
        qWarning() << "GetFilePath: No matching FileInfo found:" << time;
    }
    return resultPath;
}

QString BackupFile::getFileName(QString time)
{
    QString resultName;
    auto it = std::find_if(m_fileInfos.begin(), m_fileInfos.end(), [&](const FileInfo& fileInfo) {
        return fileInfo.birthTime == time;
    });
    if(it != m_fileInfos.end()){
        resultName = it->fileName;
    }else{
        qDebug() << "GetFileName: No matching FileInfo found." << time;
    }
    return resultName;
}

QString BackupFile::getCurrentPath()
{
    return QDir::currentPath();
}

QString BackupFile::getAppDataPath()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/AppData";
}

void BackupFile::scheduledBackup()
{
    QDir folder(getAppDataPath() + "/Andromeda/ScheduledBackup");
    if(folder.exists()){
        QFileInfoList fileList = folder.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Time);
        if(fileList.count() > 0){
            QDateTime latestTime = fileList.at(0).birthTime();
            int daysDifference = QDateTime::currentDateTime().daysTo(latestTime);
            if(daysDifference < getBackupCycle()){
                qDebug() << "ScheduledBackup: The latest backup date is" << latestTime.toString()
                         << ", there is no need for scheduled backup";
                return;
            }
        }
    }
    //  boot for the first time / daysDifference >= cycle
    QString sourcePath = QDir::currentPath() + "/config";
    QString destinationPath = getAppDataPath() + "/Andromeda/ScheduledBackup";
    QString formattedDateTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString newFolderName = "config_scheduled_backup_" + formattedDateTime;
    compressFolder(sourcePath, destinationPath, newFolderName);
    qDebug() << "ScheduledBackup: Create file" << newFolderName << ".7z";
}

void BackupFile::clearBackup()
{
    QString folderSche = getAppDataPath() + "/Andromeda/ScheduledBackup";
    int uplimit = 10;
    int lowlimit = 5;
    clearFolder(folderSche, uplimit, lowlimit);
}

