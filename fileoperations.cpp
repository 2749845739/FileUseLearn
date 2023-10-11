/** Copyright (C) Fuqing Medical and USTC BMEC RFLab - All Rights Reserved.
 ** Unauthorized copying of this file, via any medium is strictly prohibited.
 ** Proprietary and confidential.
 ** Created on \date 20230706, by \author yue.wang.
 **/
#include "fileoperations.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QByteArray>
#include <QCoreApplication>
#include "LabTool.h"
//#include <QIODevice>

FileOperations::FileOperations(QObject *parent) : QObject(parent), m_fileInfos{}
{
    qDebug() << "FileOperations()";
}

int FileOperations::getBackupCycle()
{
    int cycle;
    QJsonObject systemConfig = LabTool::ReadJsonFile("config/sysInfo.json");
    if (systemConfig.contains("backupCycle")) {
        QJsonValue cycleValue = systemConfig.value("backupCycle");
        cycle = cycleValue.toInt();
    } else {
        qWarning() << "sysInfo.json: backupCycle not exist";
    }
    return cycle;

}

int FileOperations::copyFolder(const QString &sourceFolder, const QString &destinationFolder)
{
    qDebug() << "FileOperations::copyFolder";
    QDir sourceDir(sourceFolder);
    if (!sourceDir.exists())
    {
        qWarning() << "FileOperations::Source path does not exist: " << sourceFolder;
        return -1;
    }

    QDir destinationDir(destinationFolder);
    if (!destinationDir.exists())
    {
        if (!destinationDir.mkpath(destinationFolder))
        {
            qWarning() << "FileOperations::Failed to create destination path: " << destinationFolder;
            return -2;
        }
    }

    QStringList files = sourceDir.entryList(QDir::Files);
    foreach (const QString &file, files)
    {
        QString sourceFilePath = sourceFolder + "/" + file;
        QString destinationFilePath = destinationFolder + "/" + file;
        QFile::copy(sourceFilePath, destinationFilePath);
    }

    QStringList dirs = sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &dir, dirs)
    {
        QString sourceSubDir = sourceFolder + "/" + dir;
        QString destinationSubDir = destinationFolder + "/" + dir;
        copyFolder(sourceSubDir, destinationSubDir);
    }
    return 0;
}

int FileOperations::clearFolder(const QString &folderPath, const int &uplimit, const int &lowlimit)
{
    qDebug() << "clearFolder";
    QDir folder(folderPath);
    if (!folder.exists())
    {
        qWarning() << "clearFolder::folder path does not exist: " << folderPath;
        return -1;
    }

    // 获取文件夹中所有的文件和文件夹
    QFileInfoList fileList = folder.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);
    int fileCount = fileList.count();
    qDebug() << "Total files in" << folderPath << ":" << fileCount;

    if (fileCount <= uplimit) {
        qDebug() << "clearFolder::No need to clean the folder.";
        return 0;
    }

    // 删除多余的文件
    for (int i = lowlimit; i < fileCount; ++i) {
        QString filePath = fileList.at(i).absoluteFilePath();
        QDir delFolder(filePath);
        bool removed = delFolder.removeRecursively();
        qDebug() << "Removed" << filePath << "-" << (removed ? "Success" : "Failed");
    }
    return 0;
}

void FileOperations::compressFolder(const QString &sourceFolder, const QString &destinationFolder, const QString &zipFileName)
{
    qDebug() << "compressFolder";
    //    qDebug() << "sourceFolder:"<<sourceFolder;
    //    qDebug() << "destinationFolder:"<<destinationFolder;
    //    qDebug() << "zipFileName:"<<zipFileName;

    QDir sourceFolderDir(sourceFolder);
    QDir desFolderDir(destinationFolder);
    if (!sourceFolderDir.exists() || !desFolderDir.exists())
    {
        qWarning() << "压缩失败, 文件夹不存在：" << sourceFolder << ", " << destinationFolder;
            return;
    }


    // 查看是否少库
    QStringList depends;
    depends << "7z.exe"
            << "7z.dll";
    for(const QString &file : depends)
    {
        QString strFile = QCoreApplication::applicationDirPath() + "/" + file;
        if (!QFile::exists(strFile))   qDebug() << "shao ku:"<<strFile;
    }


    QString zipFilePath = destinationFolder + "/" + zipFileName;
    QStringList arguments;
    //  7z a D:/01_File/Andromeda/FastBackup/config_fast_backup_20230701_160301 D:/Lab-Root/Package/config
    arguments << "a"<< zipFilePath << sourceFolder;
    qDebug() << "arguments:"<< arguments;

    QProcess process;
    // 7z a D:/01_File/Andromeda/FastBackup/config_fast_backup_20230706_140102 D:/Lab-Root/Package/config
    //    process.setProgram("7z");
    //    QString program = QCoreApplication::applicationDirPath() + "/7z.exe"; // 7z的路径
    //    process.setProgram(program);
    //    process.setArguments(arguments);
    //    process.start();
    //    process.waitForFinished(-1);
    // 检查命令执行状态
    //    if (process.exitCode() != 0)
    //    {
    //        qDebug() << "压缩失败：" << process.errorString();
    //    }
    //    else
    //    {
    //        qDebug() << "文件夹压缩完成：" << sourceFolder << " -> " << destinationFolder;
    //    }

    process.start("D:/Lab-Root/Package/bin/7z.exe", arguments);
    process.waitForStarted();
    process.waitForFinished();
    qDebug() << "process-01 finished:";

    // 会打出一串日志
    //    process.execute("D:/Lab-Root/Package/bin/7z.exe", arguments);
    //    process.waitForFinished();
    //    qDebug() << "process-02 finished:";

}

void FileOperations::decompressFolder(const QString &filePath, const QString &destinationPath)
{
    qDebug() << "decompressFolder";
    qDebug() << "filePath:"<<filePath;
    qDebug() << "destinationPath:"<<destinationPath;

//    QDir fileDir(filePath);
//    QDir desFolderDir(destinationPath);
//    if (!fileDir.exists() || !desFolderDir.exists())
//    {
//        qWarning() << "解压缩失败, 文件夹不存在：" << filePath << ", " << destinationPath;
//            return;
//    }

//    //    QString program = "path/to/7z.exe"; // 7z的路径
//    QString program = QCoreApplication::applicationDirPath() + "/7z.exe"; // 7z的路径

    // 构造执行命令
    //    7z x -oD:/01_File/Andromeda/restore  D:/01_File/Andromeda/ScheduledBackup/config_scheduled_backup_20230711_160301.7z -r -aoa
    QStringList arguments;
    arguments << "x" << "-o" + destinationPath << filePath << "-r" <<"-aoa";
    qDebug() << "arguments:"<< arguments;


    // 创建并启动进程
    QProcess process;
    //    process.setProgram(program);
    //    process.setArguments(arguments);
    //    process.start();
    //    process.start(QCoreApplication::applicationDirPath() + "/7z.exe", arguments);
    process.start("D:/Lab-Root/Package/bin/7z.exe", arguments);
    process.waitForFinished();
    qDebug() << "decompressFolder process-01 finished:";

    //    process.execute("D:/Lab-Root/Package/bin/7z.exe", arguments);
    //    process.waitForFinished();
    //    qDebug() << "process-02 finished:";


    // 输出执行结果
    QByteArray output = process.readAllStandardOutput();
    QByteArray error = process.readAllStandardError();
    if (output.isEmpty() && error.isEmpty())
        qDebug() << "解压缩成功！";
    else
        qDebug() << "解压缩失败：" << output << error;
}

QStringList FileOperations::getFolderFileNames(const QString &folderPath)
{
    qDebug() << "getFolderFileNames folderPath:"<<folderPath;
    QStringList fileNames{};
    QDir folderDir(folderPath);

    // 检查文件夹是否存在
    if (!folderDir.exists())
    {
        qWarning() << "文件夹不存在：" << folderPath;
            return fileNames;
    }

    QFileInfoList fileInfoList = folderDir.entryInfoList(QDir::Files);
    qDebug() << "fileInfoList.count():"<<fileInfoList.count();
    foreach(const QFileInfo & fileInfo, fileInfoList)
    {
        fileNames <<fileInfo.fileName();
    }
    return fileNames;

}

bool FileOperations::mergeFileInfos(const QString &folderPath1, const QString &folderPath2)
{
    QDir folderDir1(folderPath1);
    QDir folderDir2(folderPath2);
    //        std::vector<FileInfo> fileInfos{};
    m_fileInfos.clear();

    // 检查文件夹是否存在
    if (!folderDir1.exists() || !folderDir2.exists())
    {
        qWarning() << "合并文件夹失败，文件夹不存在：" << folderPath1<<", "<< folderPath2;
            return false;
    }

    QFileInfoList fileInfoList1 = folderDir1.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList fileInfoList2 = folderDir2.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach(QFileInfo fileInfo, fileInfoList1) {
        FileInfo fileInfoStruct;
        fileInfoStruct.fileName = fileInfo.fileName();
        fileInfoStruct.birthTime = fileInfo.birthTime().toString(DATEFORMAT);
        fileInfoStruct.source = 1;
        fileInfoStruct.path = fileInfo.absoluteFilePath();
        m_fileInfos.push_back(fileInfoStruct);
    }
    foreach(QFileInfo fileInfo, fileInfoList2) {
        FileInfo fileInfoStruct;
        fileInfoStruct.fileName = fileInfo.fileName();
        fileInfoStruct.birthTime = fileInfo.birthTime().toString(DATEFORMAT);
        fileInfoStruct.source = 2;
        fileInfoStruct.path = fileInfo.absoluteFilePath();
        m_fileInfos.push_back(fileInfoStruct);
    }

    std::sort(m_fileInfos.begin(), m_fileInfos.end(), [](const FileInfo& fileInfo1, const FileInfo& fileInfo2) {
        return fileInfo1.birthTime > fileInfo2.birthTime;
    });

    // 打印排序后的文件信息
//    foreach(FileInfo fileInfo, m_fileInfos) {
//        qDebug() << "File Name:" << fileInfo.fileName;
//        qDebug() << "Modified Time:" << fileInfo.birthTime;
//        qDebug() << "Custom Tag:" << fileInfo.source;
//        qDebug() << "-------------------------";
//    }
    return true;

}

QStringList FileOperations::getFileTimes()
{
    QStringList fileTimeList;
    foreach(FileInfo fileInfo, m_fileInfos) {
        fileTimeList.push_back(fileInfo.birthTime);
    }
    //    qDebug() << "fileTimeList:" <<fileTimeList;
    return fileTimeList;
}

QStringList FileOperations::getFileSources()
{
    QStringList fileSourceList;
    foreach(FileInfo fileInfo, m_fileInfos) {
        fileSourceList.push_back(fileInfo.source == 1 ? "Fast" : "Scheduled");
    }
    //    qDebug() << "fileSourceList:" <<fileSourceList;
    return fileSourceList;

}

QString FileOperations::getFilePath(QString time)
{
    qDebug() << "getFilePath:" <<time;
    QString resultPath;

    auto it = std::find_if(m_fileInfos.begin(), m_fileInfos.end(), [&](const FileInfo& fileInfo) {
        return fileInfo.birthTime == time;
    });

    if (it != m_fileInfos.end()) {
        resultPath = it->path;
        qDebug() << "Matching FileInfo found: " << resultPath;
    }
    else {
        qDebug() << "No matching FileInfo found.";
    }
    qDebug() << "resultPath:" <<resultPath;
    return resultPath;

}

QString FileOperations::getFileName(QString time)
{
    QString resultName;
    auto it = std::find_if(m_fileInfos.begin(), m_fileInfos.end(), [&](const FileInfo& fileInfo) {
        return fileInfo.birthTime == time;
    });
    if (it != m_fileInfos.end()) {
        resultName = it->fileName;
    }
    else {
        qDebug() << "No matching FileInfo found.";
    }
    return resultName;
}

QString FileOperations::getPackagePath()
{
    // "D:/Lab-Root/Package/bin"
    QString currentDir = QCoreApplication::applicationDirPath();
    QDir dir(currentDir);

    if (dir.cdUp()) {
        return dir.absolutePath();
    }
    else {
        return "";
    }
}

void FileOperations::scheduledBackup()
{
    qDebug() << "SLOT scheduledBackup";
    QString sourcePath = "D:/Lab-Root/Package/config";
    QString destinationPath = "D:/01_File/Andromeda/ScheduledBackup";
    QString formattedDateTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    QString newFolderName = "config_scheduled_backup_" + formattedDateTime;
    compressFolder(sourcePath, destinationPath, newFolderName);
}

void FileOperations::clearBackup()
{
    qDebug() << "SLOT clearBackup";
//    QString folderPath = QDir::currentPath() + "/";
    QString folderPath = "D:/01_File/Andromeda/ScheduledBackup";
    int uplimit = 10;
    int lowlimit = 5;
    clearFolder(folderPath, uplimit, lowlimit);
}


