#!/bin/bash

# Start program 1 in the background
echo "Starting creating online installer..."
/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/binarycreator.exe --online-only -c config/config.xml -p packages FraktalgeneratorOnlineInstaller.exe &

# Start program 2 in the background
echo "Starting creating offline installer..."
/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/binarycreator.exe --offline-only -c config/config.xml -p packages FraktalgeneratorOfflineInstaller.exe &


if [ -d "./repository" ]; then
	echo "Staring updating repository...."
	/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/repogen.exe --update -p packages repository
else
	echo "Staring creating repository...."
	/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/repogen.exe -p packages repository
fi

# Wait for program 1 and program 2 to finish
wait

# Continue with the rest of your script after program 1 and program 2 have finished
echo "Finished!"

# Prompt the user to press Enter
read -p "Press Enter to close the terminal"