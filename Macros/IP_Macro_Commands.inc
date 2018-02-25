
sub Assist(int sender_ID, int target_ID) {

	|-'Only accept kill targets from nearby players in same zone'
	/if (${Spawn[ID ${sender_ID}].ID} && ${Spawn[ID ${sender_ID}].Distance} < ${gl_comdis}) {

		|-'Only kill NPC'
		/if (${Spawn[ID ${target_ID}].Type.Equal["NPC"]}) {

			|-'Only set the assist for bots'
			/if (${Me.ID} != ${sender_ID}) /varset gl_assist_ID ${sender_ID}

			/varset gl_kill_ID ${target_ID} 

			|-'Have pets attack'
			/if (${Me.Pet.ID}) {

				/tar ID ${target_ID}
				/delay 5
				/pet back off
				/pet attack
			}
		}
	}

	/return 0
}

sub Buff(int buff_target_ID) {

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

			/loadspells ${gl_spell_set_name}

			/bc Done buffing ${Spawn[ID ${ID}].Name}.
		}
	} 

	/return
}

sub Camp() {

	/if (${Plugin["MQ2SuperDuperReallyGhettoSpeed"].Name.Length}) /squelch speed off

	/stick off

	/endmacro

	/return 0
}
	
sub Cast(string spell_name, int gem_number, int target_ID) {
	
	/declare x 					float local ${Me.X}
	/declare y 					float local ${Me.Y}
	/declare xdiff			float local 0.0
	/declare ydiff			float local 0.0
	/declare waitcount 	int 	local 0


	/if (${Me.CurrentMana} < ${Spell[${spell_name}].Mana} * 1.3) {

		/bc Not enough mana to cast.

		/varset gl_spldly 20

		/interrupt
		/return 0
	}

	/if (${gl_is_following} && ${Spawn[ID ${gl_follow_ID}].Distance} > ${gl_lshdis}) {

		/bc Interrupting spell cast -- follow target needs me.
		/squelch /target clear
		/interrupt
		/return 0
	}

	/call Memorize "${spell_name}" ${gem_number} 	
	/if (${Macro.Return}) /return 0

	:Wait
	
	/if (!${Me.SpellReady[${spell_name}]}) {

		/if (${waitcount} < 20) {
			
			/varcalc waitcount ${waitcount} + 1
			/delay 5
			/goto :Wait

		} else {

			/bc Gave up waiting for ${spell_name} to be ready
			/return 0
		}
	}

	/bc Casting ${spell_name} on ${Spawn[ID ${target_ID}].Name}

	/if (${Me.Sitting}) /stand

	/tar ID ${target_ID}
	
	/cast ${gem_number} 
	/delay 5

	:MonitorCasting

	/if (${Me.Casting.ID}) {
		
		/if (${Spawn[ID ${target_ID}].Type.Equal["Corpse"]}) {

			/bc Interrupting spell cast -- target is a corpse.
			/interrupt
			/return 0
		}

		/if (${Spawn[ID ${target_ID}].Distance} > ${gl_spldis}) {

			/bc Interrupting spell cast -- target is out of range.
			/interrupt
			/return 0
		} 

		/if (${gl_is_following} && ${Spawn[ID ${gl_follow_ID}].Distance} > ${gl_lshdis}) {

			/bc Interrupting spell cast -- follow target needs me.
			/squelch /target clear
			/interrupt
			/return 0
		}

		/varcalc xdiff ${x} - ${Me.X}
		/varcalc ydiff ${y} - ${Me.Y}

		/if (${xdiff} < -2.0 || ${xdiff} > 2.0 || ${ydiff} < -2.0 || ${ydiff} > 2.0) {

			/bc I have moved from my casting position -- Interrupting. 
			/interrupt
			/return 0
		}

		/delay 1
		/goto :MonitorCasting
	}

	/if (${Cast.Result.Equal["CAST_SUCCESS"]}) {
		/varset gl_spldly 15
		/return 1

	} else {

		/return 0
	}
}

sub Follow(int follow_ID) {

	|-'Never follow yourself'
	/if (${follow_ID} == ${Me.ID}) {

		/varset gl_follow_ID 0
		/varset gl_is_following false
		/return
	}


	|-'Proximity check'
	/if (${Spawn[ID ${follow_ID}].ID} && ${Spawn[ID ${follow_ID}].Distance} < ${gl_comdis}) {
		
		|-'Do not follow corpses'
		/if (!${Spawn[ID ${follow_ID}].Type.Equal["Corpse"]}) {

			/varset gl_kill_ID 		0
			/varset gl_assist_ID 	0

			/interrupt

			/attack off

			/varset gl_is_following true
			/varset gl_follow_ID ${follow_ID}
			/varset gl_follow_name ${Spawn[ID ${gl_follow_ID}].Name}		

			/stick ID ${gl_follow_ID} ${gl_foldis}
		}
	}

	/return
}

sub Memorize(string spell_name, int gem_number) {

	/if (${Me.Gem[${spell_name}]} != ${gem_number}) {

		/bc Memorizing ${spell_name} to slot ${gem_number}.

		/memspell ${gem_number} "${spell_name}"

		:Memorizing

		/if (${Me.Gem[${spell_name}]} != ${gem_number}) {

			/delay 5
			/goto :Memorizing
		}
		
		/stand

		/return 1
	}

	/return 0
}

sub Stop() {

	/varset gl_follow_name  0
	/varset gl_follow_ID 		0
	/varset gl_is_following false
	
	/squelch /stick off

	/return
}

sub Use_Item(string item_name) {

	/if (${FindItem[${item_name}].InvSlot}) {

		/declare sleep_time float local

		/itemnotify ${FindItem[${item_name}].InvSlot} rightmouseup

		/varcalc sleep_time ${FindItem[${item_name}].CastTime} + 0.5

		/interrupt

		/delay ${sleep_time}
	
	} else {

		/echo Could not find an item matching '${item_name}'.
	}

	/return
}