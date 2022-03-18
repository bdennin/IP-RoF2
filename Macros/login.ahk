
;Set this equal to your MQ2 directory
ippath = C:\Users\Brandon\source\repos\IP-RoF2
eqpath = C:\Users\Brandon\Downloads\everquest_rof2\everquest_rof2
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
  winwaitactive, ahk_pid %pid%
  sleep, 2000
  sendinput, %pass%
  sleep, 100
  sendinput, {enter}
  sleep, 1000
  winminimize, ahk_pid %pid%
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