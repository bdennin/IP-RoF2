
;Set this equal to your MQ2 directory
ippath = E:\GitHub\IP-RoF2
eqpath = C:\ROF2
loginx = 2050
loginy = 1140
playx = 2095
playy = 1240

#WinActivateForce

client_pids := []

loop
{
  filereadline, line, logins.txt, %a_index%
  
  if ErrorLevel
    break
  
  vals := strsplit(line, " ")

  login := % vals[1]

  run, eqgame.exe patchme /login:%login%, %eqpath%, max, pid
  sleep, 1000

  client_pids.push(pid)
}

for i, pid in client_pids
{
  while !winexist("ahk_pid" . pid)
  {
    sleep, 500
  }

  winset, disable, , ahk_pid %pid%
}

for i, pid in client_pids
{
  filereadline, line, logins.txt, %i%
  
  if ErrorLevel
    break
  
  vals := strsplit(line, " ")
  pass := % vals[2]

  winactivate, ahk_pid %pid%
  sleep, 500
  sendinput, %pass%{enter}
  sleep, 1000
}

for i, pid in client_pids
{
  while !winexist("ahk_pid" . pid)
  {
    sleep, 100
  }

  winset, enable,, ahk_pid %pid%
}

run, EQBCServer.exe, %ippath%
run, MacroQuest2.exe, %ippath%