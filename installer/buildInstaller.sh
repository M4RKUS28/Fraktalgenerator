echo "Starting creating online installer..."
/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/binarycreator.exe --online-only -c config/config.xml -p packages FraktalgeneratorOnlineInstaller.exe
echo "Finished!"
echo "Starting creating offline installer..."
/D/QtNeu/Tools/QtInstallerFramework/4.6/bin/binarycreator.exe --offline-only -c config/config.xml -p packages FraktalgeneratorOfflineInstaller.exe
echo "Finished!"
echo "Exit in 3s!"
sleep 1
echo "Exit in 2s!"
sleep 1
echo "Exit in 1s!"
sleep 1