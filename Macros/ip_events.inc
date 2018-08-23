#include ip_commands.inc

#event AA_Percent "<#*#> aa #1#"
sub Event_AA_Percent(string line, int percent) {

	/squelch /alt on ${percent}

	/return
}

#event Accept_Group "#1# invites you to join a group."
sub Event_Accept_Group(string line, string player) {

	/bc Joining group of ${player}...
	/tar clear
	/invite

	/return
}

#event Accept_Raid "#1# invites you to join a raid."
sub Event_Accept_Raid(string line, string player) {

	/bc Joining raid of ${player}...
	/raidaccept

	/return
}

#event AoE "#*# aoe"
sub Event_AoE(string line) {

	/if (${gl_is_aoe_active}) {

		/echo AoE mode deactivated.
		/varset gl_is_aoe_active false
	
	} else {

		/echo AoE mode activated.
		/varset gl_is_aoe_active true
	}

	/return
}

#event Assist "<#1#> assist on #2#"
sub Event_Assist(string line, string sender, int target_ID) {

	/call Assist ${Spawn[${sender}].ID} ${target_ID}

	/return
}

#event Buff_Request "#1# tells you, '$buffs#*#"
#event Buff_Request "#1# tells the group, '$buffs#*#"
sub Event_Buff_Request(string line, string buff_target) {

	/declare buff_target_ID 	int local -1
	/declare i 								int local

	/for i 1 to 100

		/if (${buff_target.Find[${NearestSpawn[${i}]}]}) {
			/varset buff_target_ID ${NearestSpawn[${i}].ID}
			/call Buff ${buff_target_ID}
		}

	/next i

	/if (${buff_target_ID} == -1) {

		/bc Could not find buff target: ${buff_target}...
	}

	/return
}

#event Buff "<#1#> buffs on #2#"
sub Event_Buff(string line, string sender, int target_ID) {
	
	/call Buff ${target_ID}

	/return
}

#event Character_Flag "#*# received a character flag!"
sub Event_Character_Flag(string line) {

	/bc I received a character flag!

	/return
}

#event Click "<#*#> click"
sub Event_Click(string line) {
	
	/declare i 		int local
	/declare x 		int local
	/declare y 		int local

	/for i 1 to 150

		/varcalc x ${Math.Rand[3840]} + 1
		/varcalc y ${Math.Rand[2160]} + 1

		/click left ${x} ${y}

	/next i 

	/return
}

#event Debuff "<#*#> debuffs on #1#"
sub Event_Debuff(string line, int target_ID) {

	/if (${gl_num_debuffs} == 0) /return 

	/if (${Spawn[ID ${target_ID}].ID}) {

		/interrupt

		/declare i int local

		/for i 1 to ${gl_num_debuffs}

			/call Cast ${gl_debuffs[${i},1]} ${gl_debuffs[${i},2]} ${target_ID}

		/next i
	}

	/return
}

#event Disband "<#*#> disband"
sub Event_Disband(string line) {

	/echo disbanding

	/if (${Raid.Members}) /raiddisband

	/if (${Group.Members}) /disband

	/return
}

#event End_Enrage "#*# is no longer enraged."
sub Event_End_Enrage(string line) {
	
	/if (${gl_is_melee}) /attack on

	/return
}

#event End_Macro "#*# seconds to prepare your camp."
sub Event_End_Macro(string line) {

	/call Camp

	/return
}

#event Find_Window_Text "<#*#> text #1#"
sub Event_Find_Window_Text(string line, string text) {

	/declare i 						int 		local 1
	/declare window_name 	string 	local ""

	/echo "Searching for ${text}..."

	:Iterate

  /varset window_name ${Ini[${gl_window_file},"Window_Names","Window_Name#${i}"]}

	/if (${window_name.NotEqual[NULL]}) {

		/if (${Window[${window_name}].Text.Find[${text}]}) /echo ${window_name} contains ${text}.

		/varcalc i ${i} + 1
		/goto :Iterate
	}

	/echo "Search complete."

	/return
}

#event Export_Active_Windows "<#*#> expactwin"
sub Event_Export_Active_Windows(string line, string text) {

	/declare i 						int 		local 1
	/declare window_name 	string 	local ""

	:Iterate

  /varset window_name ${Ini[${gl_window_file},"Window_Names","Window_Name#${i}"]}

	/if (${window_name.NotEqual[NULL]}) {

		/if (${Window[${window_name}].Height} > 400) /ini "Data\${Me.Name}_activewin.ini" "Windows" "Name${i}" "${window_name}"

		/varcalc i ${i} + 1
		/goto :Iterate
	}

	/echo "Export complete."

	/return
}

#event Follow "<#*#> follow #1#"
sub Event_Follow(string line, int follow_id) {

	/call Follow ${follow_id}
	
	/return
}

#event Gate "<#*#> gate"
sub Event_Gate(string line) {

	/call Use_Item ${gl_gate_item}	

	/return
} 

#event Invis "<#*#> invis"
sub Event_Invis(string line) {

	/declare i int local

	/for i 1 to ${Group}
		
		/echo ${i}
		/echo ${Group.Member[${i}]}

		/if (${gl_iscstr} && ${Spawn[Group.Member[${i}]].ID} && ${Spawn[Group.Member[${i}]].Distance} < ${gl_spldis}) {
		
			/if (!${Group.Member[${i}].Invis}) {
			
				/call Cast "Invisibility" 8 ${Group.Member[${i}].ID}
			}
		}

	/next i

	/return
}

#event Loot "<#*#> loot"
sub Event_Loot(string line) {

	/if (${SpawnCount[Corpse ${Me.Name}]} > 0) {
			
		/if (${NearestSpawn[Corpse ${Me.Name}].Distance} < ${gl_spldis}) {

			/declare i 		  int local
			/declare numitm int local	
			/declare count 	int local

			:LootCorpse

			/tar ID ${NearestSpawn[Corpse ${Me.Name}].ID}

			/corpse

			/delay 4

			/loot

			/delay 4

			/varset numitm ${Corpse.Items}

			/if (${numitm} == 0) /goto :DoneLooting

			/for i 1 to ${numitm} 

				:LootItem

				/if (!${Corpse.Open}) /goto :LootCorpse

				/varset count 0

				/itemnotify loot${i} rightmouseup
				
				/delay 1

				/if (!${Corpse.Item[${i}].ID}) {

					/varcalc count ${count} + 1

					/if (${count} >= 3) /goto :ItemLooted

				
				} else {

					/goto :LootItem
				}

				/delay 1

				:ItemLooted

			/next i

			:DoneLooting

			/delay 4

			/notify LootWnd DoneButton leftmouseup
			/notify LootWnd DoneButton leftmouseup
			/notify LootWnd DoneButton leftmouseup
		}
	}
	
	/return
}

#event Med "<#*#> med"
sub Event_Med(string line) {

	/call Toggle_Meditate
	
	/return
}

#event Rez "<#*#> rez on #1#"
sub Event_Rez(string line, int target_ID) {
	
	/if (${Me.Class.Name.Equal["Cleric"]}) {

		/if (${Spawn[ID ${target_ID}].Distance} < ${gl_spldis}) {

			/if (${Spawn[ID ${target_ID}].Type.Equal["Corpse"]}) {
				
				/tar ID ${target_ID}
				/delay 2
				/interrupt

				/if (${Bool[FindItem["Holy Symbol of the Prime Healer"]]}) {

					/itemnotify ${FindItem["Holy Symbol of the Prime Healer"].InvSlot} rightmouseup
					/delay 70
				
				} else /if (${Bool[FindItem["Water Sprinkler of Nem Ankh"]]}) {

					/itemnotify ${FindItem["Water Sprinkler of Nem Ankh"].InvSlot} rightmouseup
					/delay 40
				
				} else {

					/cast reviviscence
				}
			}
		}
	}

	/return
}

#event Speak "<#*#> speak #1# #2#"
sub Event_Speak(string line, int speak_ID, string message) {	

	/call Speak ${speak_ID} "${message}"

	/return
}

#event Start_Enrage "#1# has become ENRAGED."
sub Event_Start_Enrage(string line, string name) {
	
	/if (${gl_istank}) /attack off

	/return
} 

#event Stop "<#1#> stop"
sub Event_Stop(string line, string sender) {

	/call Stop

	/return 
}

#event Summon_Pet "<#*#> pet"
sub Event_Summon_Pet(string line) {

	/interrupt 

	/if (${Me.Class.Name.Equal["Bard"]}) /twist off

	/delay 2

	/if (${FindItem["Evolved Red Dragon Egg"].InvSlot}) {

		/itemnotify ${FindItem["Evolved Red Dragon Egg"].InvSlot} rightmouseup	
	
	} else /if (${FindItem["Red Dragon Egg"].InvSlot}) {

		/itemnotify ${FindItem["Red Dragon Egg"].InvSlot} rightmouseup	
	}
	
	/delay 16s

	/if (${Me.Class.Name.Equal["Bard"]}) /twist 2 3 4 5 6

	/return
}

#event took_damage "#*# YOU for #*#"
sub event_took_damage(string line) {
	
	/if (${Me.AbilityReady["Feign Death"]}) {

		/doability "Feign Death"
		/delay 5
		/stand

		/if (${Target.ID} && ${gl_assist_ID} != 0) /attack on

	} else /if (${Me.AbilityReady["Hide"]}) {
		
		/bc im here

		/attack off
		/delay 5
		/doability "Hide"
		/delay 5

		/if (${Target.ID} && ${gl_assist_ID} != 0) /attack on
	}

	/varset gl_damage_taken_timer 6s
	/bc [note0]

	/return
}

#event notify_combat "#*#[note0]"
sub event_notify_combat(string line) {

  /varset gl_combat_timer 6s
	/return
}

#event Use_Item "<#*#> use #1#"
sub Event_Use_Item(string line, string item_name) {

	/call Use_Item ${item_name}
	
	/return
}

#event Warp "<#1#> warp #2#"
sub Event_Warp(string line, string sender, int warp_ID) {

	/if (${Spawn[PC ${sender}].ID} == ${Me.ID}) /return

	/if (${Spawn[PC ${sender}].ID}) {

		/if (${Spawn[ID ${warp_ID}].ID}) {

			:WarpLoop

			/if (${Spawn[ID ${warp_ID}].Distance3D} > ${gl_wrpdis}) {

				/interrupt

				/tar ID ${warp_ID}
				/delay 1
				/warp t

				/goto :WarpLoop
			}
		}
	}

	/return
}
