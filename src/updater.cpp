#include "updater.h"

#include <QAbstractButton>
#include <QApplication>
#include <QFile>

Updater::Updater(QString maintananceToolPath, QString organisation, QString application, bool doAutoUpdateIfEnabled)
    : status(UPDATE_STATUS::NOT_CHECKED), updateMsgBox(nullptr), organisation(organisation), application(application), maintananceToolPath(maintananceToolPath), showMsgBox(true)
{
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

void Updater::setAutoSearchForUpdate(const bool &status)
{
    QSettings settingOwnColor(organisation, application);
    settingOwnColor.setValue("AUTO_SEARCH_FOR_UPDATE", status );
}

bool Updater::getAutoSearchForUpdateStatus()
{
    QSettings settingOwnColor(organisation, application);
    return (settingOwnColor.contains("AUTO_SEARCH_FOR_UPDATE")) ? settingOwnColor.value("AUTO_SEARCH_FOR_UPDATE").toBool() : true;
}

void Updater::setAutoShowMessageBox(const bool &status)
{
    showMsgBox = status;
}

bool Updater::showUpdateMessageBox()
{
    return zustandWechseln("showUpdateMessageBox()");
}

QString Updater::getNewVersion()
{
    return newVersion;
}

QString Updater::getError() const
{
    return error;
}

Updater::UPDATE_STATUS Updater::getStatus() const
{
    return status;
}

QString Updater::getStatusStr()
{
    switch (this->getStatus()) {
    case Updater::NOT_CHECKED:
        return "Auf Updates prüfen:";
    case Updater::NO_UPDATER:
        return "Diese Version wurde ohne Updater installiert!";
    case Updater::CHECKING:
        return "Es wird auf Updates geprüft...";
    case Updater::UP_TO_DATE:
        return "Es wird die neuste Version verwendet!";
    case Updater::UPDTAE_NEEDED: {
        if(this->getNewVersion().isEmpty())
            return "Es sind Updates verfügbar!";
        else
            return "Es ist eine Neue Version verfügbar: " + this->getNewVersion();
    }
    case Updater::UPDATING:
        return "Updater ausgeführt!";
    default:
        return "Während der Suche nach Updates ist ein Fehler aufgetreten: " + this->getError();
    }
}

bool Updater::checkForUpdates()
{
    return zustandWechseln("checkForUpdates()");
}

bool Updater::startUpdate()
{
    return zustandWechseln("startUpdate()");
}

void Updater::updateDialogButtonClicked(QAbstractButton *button)
{
    qDebug() << "updateDialogButtonClicked()";
    QMessageBox::ButtonRole role = updateMsgBox->buttonRole(button);
    zustandWechseln("updateDialogButtonClicked()", (role == QMessageBox::AcceptRole) ? "AcceptRole" : "<Declined>");
}

#ifndef Q_OS_WEB
void Updater::onUpdateCheckFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    zustandWechseln("onUpdateCheckFinished()", (exitStatus == QProcess::NormalExit && exitCode == 0) ? "QProcess::NormalExit;ExitValue==0" : "<Error>");
}
#endif




bool Updater::zustandWechseln(QString action, QString value)
{
    qDebug() << "zustandWechseln: " << action << value << " zustand: " <<this->status;
    switch (this->status) {

    case UP_TO_DATE:
        if(action == "checkForUpdates()") {
            error = "No Error";
            status = NOT_CHECKED;
            // no status changes! --> no emits        --> return
            return zustandWechseln("checkForUpdates()");
        } else {
            //Error
            return false;
        }

    case NOT_CHECKED: {
        if(action == "checkForUpdates()") {
#ifndef Q_OS_WEB
            if(!QFile(maintananceToolPath).exists()) {
                status = UPDATE_STATUS::NO_UPDATER;
                error = "Application installed without MaintenanceTool (Installer)!";
            } else {
                qDebug() << "Start the updater process";
                //connect to slot
                connect(&updaterPrz, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onUpdateCheckFinished(int,QProcess::ExitStatus)));
                // Start the updater process
                updaterPrz.start(maintananceToolPath, QStringList() << "check-updates");
                if(updaterPrz.state() == QProcess::ProcessState::NotRunning && updaterPrz.exitCode() != 0) {
                    QVector<QString> errorDescriptions = {
                       /*0*/ "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions or resources to invoke the program.",
                       /*1*/ "The process crashed some time after starting successfully.",
                       /*2*/ "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.",
                       /*3*/ "An error occurred when attempting to read from the process. For example, the process may not be running.",
                       /*4*/ "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.",
                       /*5*/ "An unknown error occurred. This is the default return value of error()."
                    };
                    error =  "Start updater failed: " + (updaterPrz.error() < 6 ? errorDescriptions.at(updaterPrz.error()) : "Qt Internal Error!");
                    status = UPDATE_STATUS::ERROR;
                } else {
                    error = "No Error";
                    status = UPDATE_STATUS::CHECKING;
                }
            }
#endif
        }  else {
            //Error
            return false;
        }
        break;
    }
    case CHECKING: {
        if(action == "onUpdateCheckFinished()") {
            qDebug() << "onUpdateCheckFinished updater process";
#ifndef Q_OS_WEB

            QString output = updaterPrz.readAllStandardOutput();

            if (value == "QProcess::NormalExit;ExitValue==0") {
                qDebug() << "updates... show msg box if enabled";

                if (output.contains("<updates>") && output.contains("</updates>")) {

                    int start = output.indexOf(" version=\"", output.indexOf("<updates>")); // start from <updates>, otherwise xml file version 1.0 is extracted!
                    if(start != -1) {
                        int ende = output.indexOf("\"", start + 10);

                        newVersion = output.mid(start + 10, ende - start - 10);
                        qDebug() << newVersion;
                    }

                    error = "No Error";
                    this->status = UPDATE_STATUS::UPDTAE_NEEDED;

                    if(showMsgBox)
                        zustandWechseln("showUpdateMessageBox()");

                } else {
                    error = "No Error";
                    this->status = UPDATE_STATUS::UP_TO_DATE;
                }
            } else {
                QString err = updaterPrz.readAllStandardOutput() + updaterPrz.readAllStandardError();
                error = "Process execution failed: " + (err.isEmpty() ? output : err) ;
                status = UPDATE_STATUS::ERROR;
            }
#endif
            break;
        } else {
            //Error
            return false;
        }

    }

    case UPDTAE_NEEDED: {

        if(action == "showUpdateMessageBox()") {
            if( !updateMsgBox) {
                updateMsgBox = new QMessageBox(QMessageBox::Information, "Update Available", "Es ist eine neuere Version verfügbar: " + newVersion + "\nBitte aktualiseren sie die Anwendung!");
                               updateMsgBox->addButton("Jetzt aktualisieren", QMessageBox::AcceptRole);
                updateMsgBox->addButton("Später aktualisieren", QMessageBox::RejectRole);
                updateMsgBox->show();
                QObject::connect(updateMsgBox, SIGNAL(buttonClicked(QAbstractButton *)), this, SLOT(updateDialogButtonClicked(QAbstractButton *)));
                return true;    // no status changes! --> no emits
            }
        } else if(action == "updateDialogButtonClicked()") {

            if(value == "AcceptRole") {
                // this->status = UPDATE_STATUS::UPDATING;
                error =  "Start Maintanance Tool for Update...";
                // qDebug() << "Start Maintanance Tool for Update... " << value;
                startUpdate();
                // qDebug() << " after: " << this->getError();
            } else {
                error = "No Error";
                this->status = UPDATE_STATUS::UPDTAE_NEEDED;
            }
            if(updateMsgBox) {
                updateMsgBox->deleteLater();
                updateMsgBox = nullptr;
            }

            emit statusChanged();


        } else if(action == "startUpdate()") {
            // qDebug() << "UPDTAE_NEEDED::startUpdate";


#ifndef Q_OS_WEB
            if(!QFile(maintananceToolPath).exists()) {
                status = UPDATE_STATUS::NO_UPDATER;
                error =  "Not installed with updater!";
                // qDebug() << error;
            } else {
                // qDebug() << "Start MaintenanceTool..." << maintananceToolPath;
                if(!QProcess::startDetached(maintananceToolPath)) {
                    status = UPDATE_STATUS::ERROR;
                    error =  "Start MaintenanceTool failed";
                    // qDebug() << error;
                } else {
                    error = "No Error";
                    status = UPDATE_STATUS::UPDATING;
                }
            }
            break;
#endif

        } else {
            //Error
            return false;
        }
        break;
    }
    case UPDATING:
        return false;
    case ERROR:
        return false;
    case NO_UPDATER:
        return false;
    }

    emit statusChanged();
    return true;
}

