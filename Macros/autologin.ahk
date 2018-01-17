
;Set this equal to your MQ2 directory
ippath = C:\Users\Jimbob\Downloads\IP-ROF2

numclients := 6

loop, %numclients%
{
  run, eqgame.exe patchme /login:bdens0%A_Index%, D:\ROF2
  sleep, 1000
}

sleep, 12000

winget, windows, list, EverQuest

Loop, %windows%
{
  winactivate, % "ahk_id" windows%A_Index%
  sendinput, angel123
  sendinput, {enter}
  sleep, 1000
}

run, EQBCServer.exe, %ippath%
run, MacroQuest2.exe, %ippath%
