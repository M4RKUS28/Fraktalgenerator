#!/bin/bash

if [ -d "./repository" ]; then
	echo "Staring updating repository...."
	/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/repogen.exe --update -p packages repository
else
	echo "Staring creating repository...."
	/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/repogen.exe -p packages repository
fi

echo "Finished!"
echo "Exit in 3s!"
sleep 1
echo "Exit in 2s!"
sleep 1
echo "Exit in 1s!"
sleep 1