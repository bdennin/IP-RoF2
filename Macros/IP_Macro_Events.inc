
sub Check_Events() {
	
	/doevents AA_Percent
	/doevents Accept_Group
	/doevents Accept_Raid
	/doevents AoE
	/doevents Assist
	/doevents Buff
	/doevents Buff_Request
	/doevents Character_Flag
	/doevents Click
	/doevents Cure
	/doevents Debuff
	/doevents Disband
	/doevents End_Enrage
	/doevents End_Macro
	/doevents Follow
	/doevents Gate
	/doevents Invis
	/doevents Loot
	/doevents Med
	/doevents PetAssist
	/doevents Rez
	/doevents Speak
	/doevents Start_Enrage
	/doevents Stop
	/doevents Stuns
	/doevents Summon_Pet
	/doevents Take_Damage
	/doevents Use_Item
	/doevents Warp

	/return
}

#event AA_Percent "<#*#> aa #1#"
sub Event_AA_Percent(string line, int percent) {

	/squelch /alt on ${percent}

	/return
}

#event Accept_Group "#*# invites you to join a group."
sub Event_Accept_Group(string line) {

	/bc Joining group...
	/tar clear
	/invite

	/return
}

#event Accept_Raid "#*# invites you to join a raid."
sub Event_Accept_Raid(string line) {

	/bc Joining raid...
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

	/bc '${buff_target}' ID ${Spawn['${buff_target}'].ID}

	/call Buff ${Spawn["${buff_target}"].ID}

	/return
}

#event Buff "<#1#> buffs on #2#"
sub Event_Buff(string line, string sender, int ID) {

	/if (${gl_num_comm_buffs} == 0) /return

	/if (${Spawn[PC ${sender}].ID} && ${Spawn[${sender}].Distance} < ${gl_comdis}) {

		/if (${Spawn[ID ${ID}].ID} && ${Spawn[ID ${ID}].Distance} < ${gl_spldis}) {

			/bc Buffing ${Spawn[ID ${ID}].Name}.

			/declare i int local

			/for i 1 to ${gl_num_comm_buffs}

				:WaitMana

				/if (${Me.CurrentMana} < ${Spell[${gl_comm_buffs[${i},1]}].Mana} + 300) {

					/if (!${Me.Sitting}) /sit
					/delay 6s
					/goto :WaitMana
				}

				/call Cast ${gl_comm_buffs[${i},1]} ${gl_comm_buffs[${i},2]} ${ID}

			/next i

			/loadspells ${gl_splset}

			/bc Done buffing ${Spawn[ID ${ID}].Name}.
		}
	} 

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

#event Cure "#1# tells the group, '$cure#*#"
sub Event_Cure(string line, string name) {	

	/bc ${name.Arg[2,1]}
	/bc ${Spawn[PC ${name.Arg[2,1]}]}
	/call Cast "Eradicate Disease" 8 ${Spawn[PC line.Arg[1, ' ']]}

	/return
}

#event Debuff "<#*#> debuffs on #1#"
sub Event_Debuff(string line, int ID) {

	/if (${gl_num_debuffs} == 0) /return 

	/if (${Spawn[ID ${ID}].ID}) {

		/interrupt

		/declare i int local

		/for i 1 to ${gl_num_debuffs}

			/call Cast ${gl_debuffs[${i},1]} ${gl_debuffs[${i},2]} ${ID}

		/next i
	}

	/return
}

#event Disband "<#*#> disband"
sub Event_Disband(string line) {

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

#event Follow "<#*#> follow #1#"
sub Event_Follow(string line, int ID) {

	/call Follow ${ID}
	
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

			/declare i 		int local
			/declare numitm int local	
			/declare count 	int local

			:LootCorpse

			/tar ID ${NearestSpawn[Corpse ${Me.Name}].ID}

			/delay 5

			/corpse

			/loot

			/delay 10

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

			/delay 1s

			/notify LootWnd DoneButton leftmouseup
		}
	}
	
	/return
}

#event Med "<#*#> med"
sub Event_Med(string line) {

	/if (${gl_shdmed}) {

		/if (${gl_ismedg}) {

			/stand
		
			/varset gl_ismedg false

			/bc I am no longer meditating.

		} else {

			/varset gl_ismedg true

			/bc I am meditating.
		}
	}
	
	/return
}

#event Rez "<#*#> rez on #1#"
sub Event_Rez(string line, int ID) {
	
	/if (${Me.Class.Name.Equal["Cleric"]}) {

		/if (${Spawn[ID ${ID}].Distance} < ${gl_spldis}) {

			/if (${Spawn[ID ${ID}].Type.Equal["Corpse"]}) {
				
				/tar ID ${ID}
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
sub Event_Speak(string line, int ID, string message) {	

	/if (${Spawn[ID ${ID}].ID} && ${Spawn[ID ${ID}].Distance} < ${gl_comdis}) {

		:MoveToTarget

		/if (${Spawn[ID ${ID}].ID} && ${Spawn[ID ${ID}].Distance} > ${gl_spkdis}) {

			/moveto ID ${ID}
			/delay 5

			/goto :MoveToTarget
		}

		/tar ID ${ID}
		
		/delay 1

		/hail
		/say ${message}	
	}

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

#event Take_Damage "#1# YOU for #*# points of damage."
sub Event_Take_Damage(string line) {

	/if (!${gl_istank}) {

		/bc i have agro

		/if (${Me.AbilityReady["Feign Death"]}) {

			/doability "Feign Death"
			/delay 5
			/stand

			/if (${Target.ID} && ${gl_isastg} != 0) /attack on

		} else /if (${Me.AbilityReady["Hide"]}) {
			
			/bc im here

			/attack off
			/delay 5
			/doability "Hide"
			/delay 5

			/if (${Target.ID} && ${gl_isastg} != 0) /attack on
		}

		/varset gl_dmgtmr 0s
	}

	/return
}

#event Use_Item "<#*#> use #1#"
sub Event_Use_Item(string line, string item_name) {

	/call Use_Item ${item_name}
	
	/return
}

#event Warp "<#1#> warp #2#"
sub Event_Warp(string line, string sender, int ID) {

	/if (${Spawn[PC ${sender}].ID} == ${Me.ID}) /return

	/if (${Spawn[PC ${sender}].ID}) {

		/if (${Spawn[ID ${ID}].ID}) {

			:WarpLoop

			/if (${Spawn[ID ${ID}].Distance3D} > ${gl_wrpdis}) {

				/interrupt

				/tar ID ${ID}
				/delay 1
				/warp t

				/goto :WarpLoop
			}
		}
	}

	/return
}
