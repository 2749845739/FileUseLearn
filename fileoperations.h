/** Copyright (C) Fuqing Medical and USTC BMEC RFLab - All Rights Reserved.
 ** Unauthorized copying of this file, via any medium is strictly prohibited.
 ** Proprietary and confidential.
 ** Created on \date 20230706, by \author yue.wang.
 **/
#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include <QObject>
#include <QDateTime>

#define DATEFORMAT "yyyy-MM-dd    hh:mm:ss"

class FileOperations : public QObject
{
    Q_OBJECT
public:
    struct FileInfo {
        QString fileName;
        QString birthTime;
        QString path;
        int source;
    };

    explicit FileOperations(QObject *parent = nullptr);
    int getBackupCycle();
    /**
     * @brief copyFolder
     * @param sourcePath
     * @param destinationPath
     * @return  0: success
     *         -1: Source path does not exist
     *         -2: Failed to create destination path
     */
    Q_INVOKABLE int copyFolder(const QString &sourcePath, const QString &destinationPath);
    /**
     * @brief clearFolder当文件夹内文件数量超过uplimit时，清理文件夹，只保留最新的lowlimit个文件/文件夹
     * @param folderPath 待清理文件夹路径
     * @param uplimit    文件数量上限
     * @param lowlimit   文件数量下限
     * @return 0         success
     *         -1        文件夹路径不存在
     */
    Q_INVOKABLE int clearFolder(const QString &folderPath, const int &uplimit, const int &lowlimit);
    Q_INVOKABLE void compressFolder(const QString &sourcePath, const QString &destinationPath, const QString &zipFileName);
    Q_INVOKABLE void decompressFolder(const QString &filePath, const QString &destinationPath);
    Q_INVOKABLE QStringList getFolderFileNames(const QString &folderPath);
    Q_INVOKABLE bool mergeFileInfos(const QString &folderPath1, const QString &folderPath2);
    Q_INVOKABLE QStringList getFileTimes();
    Q_INVOKABLE QStringList getFileSources();
    Q_INVOKABLE QString  getFilePath(QString time);
    Q_INVOKABLE QString  getFileName(QString time);
    Q_INVOKABLE QString getPackagePath();

private:
    std::vector<FileInfo> m_fileInfos;


public slots:
    void scheduledBackup();
    void clearBackup();

};

#endif // FILEOPERATIONS_H
