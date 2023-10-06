#ifndef UPDATER_H
#define UPDATER_H



#include <QString>
#include <QSettings>
#include <QProcess>
#include <QMessageBox>


//#define Q_OS_WEB



class Updater : public QObject
{
    Q_OBJECT
public:
    enum UPDATE_STATUS {
        NOT_CHECKED,
        NO_UPDATER,
        CHECKING,
        UP_TO_DATE,
        UPDTAE_NEEDED,
        UPDATING,
        ERROR
    } status;

    Updater(bool doAutoUpdateIfEnabled = true);
    ~Updater();

    void setAutoSearchForUpdate(bool status);
    bool getAutoSearchForUpdateStatus();
    void setAutoShowMessageBox(bool status);

    bool updateExists();
    QString getNewVersion();
    UPDATE_STATUS getStatus() const;

    void checkForUpdates();
    void startUpdate();

    QString getError() const;

private:
#ifndef Q_OS_WEB
    QProcess updaterPrz;
#endif
    QMessageBox * updateMsgBox;

signals:
    emit void statusChanged();


public slots:
    void updateDialogButtonClicked(QAbstractButton *button);

#ifndef Q_OS_WEB
    void onUpdateCheckFinished(int exitCode, QProcess::ExitStatus exitStatus);
#endif

private:
    QString newVersion, error;
    bool showMsgBox;
};

#endif // UPDATER_H
