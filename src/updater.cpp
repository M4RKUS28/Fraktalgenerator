#include "updater.h"

#include <QAbstractButton>
#include <QApplication>
#include <QFile>

Updater::Updater(bool doAutoUpdateIfEnabled)
{
    status = UPDATE_STATUS::NOT_CHECKED;
    showMsgBox = true;
    qDebug() << "Updater()";

    //load color from qsetting:


    if(doAutoUpdateIfEnabled && getAutoSearchForUpdateStatus()) {
        this->checkForUpdates();
    }

#ifdef Q_OS_WEB
    status = UPDATE_STATUS::NO_UPDATER;
#endif
}

Updater::~Updater()
{
    qDebug() << "~Updater()";
#ifndef Q_OS_WEB
    if(updaterPrz.state() == QProcess::Running) {
        qDebug() << "Kill updater";
        updaterPrz.kill();
        if(!updaterPrz.waitForFinished()) {
            updaterPrz.terminate();
            qDebug() << "terminate updater";
        }
    }
#endif


}

void Updater::setAutoSearchForUpdate(bool status)
{
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    settingOwnColor.setValue("AUTO_SEARCH_FOR_UPDATE", status );
}

bool Updater::getAutoSearchForUpdateStatus()
{
    QSettings settingOwnColor("Markus@W-Sem_2022", "Fraktalgenerator");
    return (settingOwnColor.contains("AUTO_SEARCH_FOR_UPDATE")) ? settingOwnColor.value("AUTO_SEARCH_FOR_UPDATE").toBool() : true;
}

void Updater::setAutoShowMessageBox(bool status)
{
    showMsgBox = status;
}

QString Updater::getNewVersion()
{
    return newVersion;
}


void Updater::checkForUpdates()
{
#ifndef Q_OS_WEB
    if(updaterPrz.state() != QProcess::NotRunning)
        return;

    if(!QFile(QApplication::applicationDirPath() + "/../../../FraktalgeneratorMaintenanceTool.exe").exists()) {
        status = UPDATE_STATUS::NO_UPDATER;
        error = "Not installed with updater!";
        emit statusChanged();
        return;
    } else
        qDebug() << "Start the updater process";

    //connect to slot
    connect(&updaterPrz, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onUpdateCheckFinished(int,QProcess::ExitStatus)));

    // Start the updater process
    updaterPrz.start( QApplication::applicationDirPath() + "/../../../FraktalgeneratorMaintenanceTool.exe", QStringList() << "check-updates");
    if(updaterPrz.state() == QProcess::ProcessState::NotRunning) {
        status = UPDATE_STATUS::ERROR;
        error =  "Start updater failed";
        emit statusChanged();
        return;
    }
    status = UPDATE_STATUS::CHECKING;
    emit statusChanged();

#endif
}

void Updater::startUpdate()
{
#ifndef Q_OS_WEB
    status = UPDATE_STATUS::UPDATING;

    if(!QFile(QApplication::applicationDirPath() + "/../../../FraktalgeneratorMaintenanceTool.exe").exists()) {
        status = UPDATE_STATUS::NO_UPDATER;
        error =  "Not installed with updater!";
        emit statusChanged();
        return;
    } else
        qDebug() << "Start FraktalgeneratorMaintenanceTool...";

    if(!QProcess::startDetached(QApplication::applicationDirPath() + "/../../../FraktalgeneratorMaintenanceTool.exe")) {
        status = UPDATE_STATUS::ERROR;
        error =  "Start FraktalgeneratorMaintenanceTool failed";
        emit statusChanged();
        return;
    }
    emit statusChanged();
#endif
}

void Updater::updateDialogButtonClicked(QAbstractButton *button)
{
    QMessageBox::ButtonRole role = updateMsgBox->buttonRole(button);
    if (role == QMessageBox::AcceptRole) {
        this->status = UPDATE_STATUS::UPDATING;
        error =  "want updating...";
        startUpdate();
    } else if (role == QMessageBox::RejectRole) {
        this->status = UPDATE_STATUS::UPDTAE_NEEDED;
    }
    emit statusChanged();
}

#ifndef Q_OS_WEB
void Updater::onUpdateCheckFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "onUpdateCheckFinished updater process";
    QString output = updaterPrz.readAllStandardOutput();

    if (exitStatus == QProcess::NormalExit && exitCode == 0) {
        qDebug() << "updates... show msg box";

        if (output.contains("<updates>") && output.contains("</updates>")) {
            this->status = UPDATE_STATUS::UPDTAE_NEEDED;

            int start = output.indexOf("Desktop App\" version=\"");
            if(start != -1) {
                int ende = output.indexOf("\"", start + 22);

                newVersion = output.mid(start + 22, ende - start - 22);
                qDebug() << newVersion;
            }

            if(showMsgBox) {
                updateMsgBox = new QMessageBox(QMessageBox::Information, "Update Available", "Es ist eine neuere Version verfügbar: " + newVersion + "\nBitte aktualiseren sie die Anwendung!");
                updateMsgBox->addButton("Update Now", QMessageBox::AcceptRole);
                updateMsgBox->addButton("Update Later", QMessageBox::RejectRole);
                updateMsgBox->show();
                connect(updateMsgBox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(updateDialogButtonClicked(QAbstractButton*)));
            }

        } else {
            this->status = UPDATE_STATUS::UP_TO_DATE;
        }
    } else {
        status = UPDATE_STATUS::ERROR;
        QString err = updaterPrz.readAllStandardError();
        error = "Process exec failed: " + (err.isEmpty() ? output : err) ;
    }

    emit statusChanged();
}

QString Updater::getError() const
{
    return error;
}
#endif

Updater::UPDATE_STATUS Updater::getStatus() const
{
    return status;
}
