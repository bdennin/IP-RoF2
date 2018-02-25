#include IP_Macro_Commands.inc

sub Automation_Setup() {
	
	/declare gl_wait_timer 							timer 	outer 0.0

	/declare gl_pack_leader						string 	outer "Rancor"
	/declare gl_friendly_warp_target 	string 	outer "Blaze"
	
	/declare gl_is_auto_active 				bool 		outer	false


	/return
}

sub Automation_Loop() {

	/declare target_ID 	int local 0
	/declare i 					int local 0

	/doevents Auto_Enable

	/if (${Me.Name.NotEqual[${gl_pack_leader}]}) /return

	/if (${gl_is_auto_active}) {

		/call Gather_Targets

		/varset gl_wait_timer 300.0

		/tar ID ${NetBots[${gl_friendly_warp_target}].ID}
		/warp t
		/delay 30
		/bc warp ${Me.ID}

		:Loot4

		/if (!${gl_wait_timer}) /return

		/if (${NearestSpawn[1, Corpse].ID} && ${NearestSpawn[1, Corpse].Distance} < 50.0) {

			/squelch /tar ID ${NearestSpawn[1, Corpse].ID}
			/squelch /warp t
			/loot
			/delay 3

			/for i 1 to ${Corpse.Items} 

				/if (${Corpse.Item[${i}].Name.Find["Token"]}) {

					/itemnotify loot${i} rightmouseup
					/echo it has a shiny!
					/delay 4
				}

			/next i
				
			/notify LootWnd DoneButton leftmouseup
			/goto :Loot
		}
	}

	/if (${gl_wait_timer}) {
		/echo delaying for a sec

		/delay 10
		/goto :Loot
	}

	/return
}

sub Gather_Targets() {

	/declare i 						int local 0
	/declare num_gathered int local 0
	/declare num_targets 	int local 65
	/declare target_ID 		int local 0
	
	/if (${SpawnCount} < ${num_targets}) {
		/echo Waiting for enemies.
		/delay 30
		/return
	}

	/for i 1 to ${num_targets} 

		/tar ID ${NearestSpawn[${i}, NPC].ID}
		/delay 3
		/warp t
		/echo ${i}

	/next i

	/echo done
	:Done

	/return
}

#event Auto_Enable "<#*#> auto"
sub Event_Auto_Enable(string line) { 

	/if (${gl_is_auto_active}) {

		/varset gl_is_auto_active false
		/varset gl_is_aoe_active 	false

	} else {
		
		/varset gl_is_auto_active true
		/varset gl_is_aoe_active 	true

		/if (${gl_shdmed}) /varset gl_is_medding true

		/squelch /hidecorpse all
		/squelch /hidecorpse looted
	}

	/return
}