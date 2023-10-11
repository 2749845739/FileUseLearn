#ifndef FILE_H
#define FILE_H

#include <QObject>

class File : public QObject {
  Q_OBJECT
public:
  //    explicit File(QObject *parent = nullptr);
  File();
  // as a component, expose property to qml
  // after set is called, get must execute
  Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

  // why need const?
  QString source() const;
  void setSource(const QString &source);

  QString text() const;
  /**
   * @brief setText
   * @param write to file
   */
  void setText(const QString &text);

signals:
  // signal do not need to be implemented, just for emit
  void sourceChanged();
  void textChanged();

private slots:
  void readFile();

private:
  QString m_source;
  QString m_text;
};

#endif // FILE_H
