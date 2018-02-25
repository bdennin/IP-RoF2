
;Set this equal to your MQ2 directory
ippath = C:\Users\brand\Documents\GitHub\IP-RoF2
eqpath = C:\ROF2

numclients := 0

Loop
{
  filereadline, line, logins.txt, %a_index%
  
  if ErrorLevel
    break
  
  vals := strsplit(line, " ")

  login := % vals[1]

  run, eqgame.exe patchme /login:%login%, %eqpath%

  numclients++

  sleep, 1000
}

sleep, 6000

winget, windows, list, EverQuest

Loop %windows%
{
  filereadline, line, logins.txt, %a_index%
  
  if ErrorLevel
    break
  
  vals := strsplit(line, " ")

  pass := % vals[2]

  winactivate, % "ahk_id" windows%a_index%
  sleep, 100
  sendinput, %pass%
  sleep, 100
  sendinput, {enter}
  sleep, 800
}

run, EQBCServer.exe, %ippath%
run, MacroQuest2.exe, %ippath%