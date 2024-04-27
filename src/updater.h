#ifndef UPDATER_H
#define UPDATER_H



#include <QString>
#include <QSettings>
#include <QProcess>
#include <QMessageBox>


#define Q_OS_WEB



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

    Updater(QString maintananceToolPath, QString organisation, QString application, bool doAutoUpdateIfEnabled = true);
    ~Updater();

    //Getter
    bool getAutoSearchForUpdateStatus();
    bool updateExists();
    QString getNewVersion();
    QString getError() const;
    UPDATE_STATUS getStatus() const;
    QString getStatusStr();

    //Setter
    void setAutoSearchForUpdate(const bool & status);
    void setAutoShowMessageBox(const bool & status);
    bool showUpdateMessageBox();

    //
    bool checkForUpdates();
    bool startUpdate();

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

    bool zustandWechseln(QString action, QString value = "");
    QString newVersion;
    QString error;
    QString organisation;
    QString application;

    QString maintananceToolPath;
    bool showMsgBox;
};

#endif // UPDATER_H
