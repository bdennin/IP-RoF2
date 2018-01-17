#include IP_Macro_Commands.inc

sub Automation_Setup() {
	
	/declare m_packleader		string 	outer "Rancor"

	/declare m_relocatetimer 	timer	outer 1m
	/declare m_bufftimer		timer 	outer 1000m

	/declare m_begmed 			int		outer 20
	/declare m_endmed 			int		outer 90
	/declare m_x 				int 	outer 0
	/declare m_y 				int 	outer 0
	/declare m_z 				int 	outer 0
	/declare m_lastx 			int 	outer 0
	/declare m_lasty 			int 	outer 0
	/declare m_lastz 			int 	outer 0
	/declare m_numoors 			int 	outer 0

	/declare m_autoing 			bool 	outer false
	/declare m_medding 			bool  	outer false	

	/return
}

sub Automation_Loop() {

	/if (${Me.Name.NotEqual[${m_packleader}]}) /return

	/doevents Auto
	/doevents Combat
	/doevents OOR	
	/doevents Whisper

	/if (!${m_autoing}) /return

	/if (!${m_relocatetimer} || ${m_numoors} > 9 && !${m_medding})  {

		/echo Auto :: Relocating
		/call Relocate
		/return
	}

	/call Check_Auto_Nearby
	/if (${Macro.Return} == 1) /return

	/call Check_Auto_Med
	/if (${Macro.Return} == 1) /return

	/call Check_Auto_Buffs
	/if (${Macro.Return} == 1) /return

	/call Move

	/return
}

sub Check_Auto_Nearby() {

	/if (${NearestSpawn[NPC].Distance} < 30) {

		/tar ID ${NearestSpawn[NPC].ID}

		/delay 1

		/stick ID ${Target.ID} ${gl_assdis}

		/bc assist on ${Target.ID}

		/attack on

		/delay 3s

		/return 1
	} 

	/tar ID ${NearestSpawn[NPC].ID}
	/delay 1
	/face

	/return	0
}

sub Check_Auto_Med() {

	/ECHO CHECKIN MED

	/declare cnctdb 	string 	local 	${NetBots.Client}
	/declare netbot 	string 	local 	0
	/declare manpct		int 	local 	0
	/declare bothps		int 	
	/declare i 			int 	local 	0

	/if (${m_medding}) {

		/for i 1 to ${NetBots.Counts} 

			/varset netbot ${cnctdb.Arg[${i}]}
			/varset manpct ${NetBots[${netbot}].PctMana}

			/if (${NetBots[${netbot}].Class.Name.NotEqual["Bard"]} && ${manpct} != 0 && ${manpct} < ${m_endmed}) {

				/return 1
			}

		/next i

		/echo Auto :: Ending meditation.
		/varset m_medding false
		/varset gl_ismedg false
	
	} else {

		/for i 1 to ${NetBots.Counts} 

			/varset netbot ${cnctdb.Arg[${i}]}
			/varset manpct ${NetBots[${netbot}].PctMana}

			/ECHO ${netbot} ${manpct}

			/if (${manpct} > 0 && ${manpct} < ${m_begmed}) {

				/echo Auto :: Beginning meditation.

				/varset m_medding true
				/varset gl_ismedg true
				/return 1
			}

		/next i
	} 

	/return 0
}

sub Check_Auto_Buffs() {
	
	/if (!${m_bufftimer}) {

		/echo Auto :: Requesting buffs.

		/varset m_bufftimer 30m

		/bc buffs on ${Me.ID}

		/delay 30s
	}

	/return 0
}

sub Move() {

	/varset m_lastx ${m_x}
	/varset m_lasty ${m_y}
	/varset m_lastz ${m_z}

	/varset m_x ${Me.X}
	/varset m_y ${Me.Y}
	/varset m_z ${Me.Z}

	/if (${Math.Calc[${Math.Abs[${m_x} - ${m_lastx}]} + ${Math.Abs[${m_y} - ${m_lasty}]}]} < 20) /call Turn_Rand

	/call Move_Forward 30 false

	/return
}

sub Relocate() {

	/attack off
	//target clear
	/bc follow ${Me.ID}

	/call Turn_Rand
	/call Move_Forward 200 true

	/varset m_relocatetimer 1m
	/varset m_numoors 0

	/return
}

sub Turn_Rand() {

	/declare rand int local

	/varset rand ${Math.Rand[5]}

	/if (${rand} == 0) /call Turn_Right
	/if (${rand} == 1) /call Turn_Left
	/if (${rand} == 2) /call Turn_Around
	/if (${rand} == 3) /call Turn_Right
	/if (${rand} == 4) /call Turn_Left

	/return
}

sub Turn_Right() {

	/keypress d hold
	/call Steady ${Math.Rand[10]}
	/keypress d
	/call Move_Forward ${Math.Rand[25]}

	/return
}

sub Turn_Left() {

	/keypress a hold
	/call Steady ${Math.Rand[10]} 
	/keypress a
	/call Move_Forward ${Math.Rand[25]}

	/return
}

sub Turn_Around() {

	/keypress a hold
	/call Steady 2
	/keypress a

	/return
}

sub Move_Forward(int delay, bool break) {

	/keypress w hold
	/call Steady ${delay} ${break}
	/keypress w

	/return
}

sub Steady(int delay, bool break) {

	/declare i int local

	/for i 1 to ${delay}

		/delay 1

		/if (!${break} && ${NearestSpawn[NPC].Distance} < 100) /return

	/next i

	/return
}

#event Auto "<#*#> auto"
sub Event_Auto(string line) {

	/if (!${m_autoing}) {

		/varset m_autoing true

		/if (${Me.Name.Equal[${m_packleader}]}) {

			/bc follow ${Me.ID}
			/varset gl_ismedg false
		}

		/echo Automation enabled.
		
		} else {

			/if (${gl_iscstr}) {

				/varset gl_ismedg true

			} else {

				/varset gl_ismedg false
			}

			/varset m_autoing 	false
			/varset gl_incmbt 	false
			/varset gl_kill_ID 	0
			/stick off
			/attack off

			/echo Automation disabled.
	}

	/return
}

#event Combat "You #*# a #*# damage."
#event Combat "#*# YOU for #*# points of damage."
sub Event_Combat(string line) {

	/varset m_numoors 0
	/varset m_relocatetimer 1m

	/return
}

#event OOR "#*# see your target."
#event OOR "#*# too far away, get closer!"
sub Event_OOR(string line) {

	/varcalc m_numoors ${m_numoors} + 1

	/return
}

#event Whisper "#*#tells you,#*#"
sub Event_Whisper(string line) {

	|/if (${Me.Name.Equal[${m_packleader}]}) /beep

	/return
}
