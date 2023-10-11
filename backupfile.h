/** Copyright (C) Fuqing Medical and USTC BMEC RFLab - All Rights Reserved.
 ** Unauthorized copying of this file, via any medium is strictly prohibited.
 ** Proprietary and confidential.
 ** Created on 20230706, by yue.wang.
 **/
#ifndef BACKUPFILE_H
#define BACKUPFILE_H

#include <QObject>
#include <QDateTime>

#define DATEFORMAT "yyyy-MM-dd    hh:mm:ss"

class BackupFile : public QObject
{
    Q_OBJECT
public:
    struct FileInfo {
        QString fileName;
        QString birthTime;
        QString path;
        int source;
    };

    explicit BackupFile(QObject *parent = nullptr);
    /**
     * @brief getBackupCycle-get scheduled backup cycle from sysInfo.json
     * @return
     */
    Q_INVOKABLE int getBackupCycle();
    /**
     * @brief clearFolder: When the number of files in the folder exceeds the uplimit, clean the folder and only keep the latest lowlimit files/folders
     * @param folderPath   Path of folder to be cleaned
     * @param uplimit      Maximum number of files
     * @param lowlimit     Minimum number of files
     * @return 0           Success
     *         -1          The folder path does not exist
     */
    Q_INVOKABLE int clearFolder(const QString &folderPath, const int &uplimit, const int &lowlimit);
    /**
     * @brief compressFolder: Compress the folder to the specified location and specify the compressed package name
     * @param sourcePath      The path of the folder to be compressed
     * @param destinationPath Destination location for storing compressed packages
     * @param zipFileName
     * @return 0              Success
     *         -1             Path does not exist
     *         -2             Missing dependency library
     */
    Q_INVOKABLE int compressFolder(const QString &sourcePath, const QString &destinationPath, const QString &zipFileName);
    /**
     * @brief decompressFolder: Decompress the compressed package to the designated location (overlay)
     * @param filePath          The name of the compressed package
     * @param destinationPath   Specify decompression location
     * @return 0                Success
     *         -1               The compressed package does not exist
     *         -2               Path does not exist
     *         -3               Missing dependency library
     */
    Q_INVOKABLE int decompressFolder(const QString &filePath, const QString &destinationPath);
    /**
     * @brief mergeFileInfos: Merge file information from two folders and sort them
     * @param folderPath1
     * @param folderPath2
     * @return
     */
    Q_INVOKABLE bool mergeFileInfos(const QString &folderPath1, const QString &folderPath2);
    /**
     * @brief getFileTimes: Obtain the creation time of all configuration files
     * @return
     */
    Q_INVOKABLE QStringList getFileTimes();
    /**
     * @brief getFileSources: Obtain the source folder for all configuration files
     * @return
     */
    Q_INVOKABLE QStringList getFileSources();
    /**
     * @brief getFilePath: Obtain the corresponding file path based on the creation time of the configuration file
     * @param time         The creation time of the configuration file
     * @return             The path of configuration file
     */
    Q_INVOKABLE QString  getFilePath(QString time);
    /**
     * @brief getFileName: Obtain the corresponding file name based on the creation time of the configuration file
     * @param time         The creation time of the configuration file
     * @return             The name of configuration file
     */
    Q_INVOKABLE QString  getFileName(QString time);
    Q_INVOKABLE QString  getCurrentPath();
    Q_INVOKABLE QString  getAppDataPath();

private:
    std::vector<FileInfo> m_fileInfos;

public slots:
    void scheduledBackup();
    void clearBackup();
};

#endif // BACKUPFILE_H
