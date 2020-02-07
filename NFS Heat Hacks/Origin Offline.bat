@echo off
@rem This disables your network adapter to force Origin offline
echo Playing Offline!
timeout 1 > nul
echo Disabling Internet...
timeout 1 > nul
netsh interface set interface Ethernet admin=disable
@rem Copies your backed up modded save to game directory
echo Starting NFS Heat...
timeout 1 > nul

@rem Change directory to your Heat executable and run Heat
D:
cd D:\Program Files (x86)\Origin Games\Need For Speed Heat
echo Running NFS Heat...
start NeedForSpeedHeat.exe

@rem Press any key to enable your network adapter once you exit the game
timeout 30 > nul
pause>nul|set/p =Press any key to re-enable internet...
netsh interface set interface Ethernet admin=enable
echo Internet enabled... Exiting...
timeout 3 > nul

