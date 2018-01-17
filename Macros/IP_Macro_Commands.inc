
sub Assist(int sender_ID, int target_ID) {

	|-'Only accept kill targets from nearby players in same zone'
	/if (${Spawn[ID ${sender_ID}].ID} && ${Spawn[ID ${sender_ID}].Distance} < ${gl_comdis}) {

		|-'Todo: Add more specific filter'
		|-'Only kill NPC'
		/if (${Spawn[ID ${target_ID}].Type.Equal["NPC"]}) {

			|-'Set player that is to be assisted'
			/varset gl_assist_ID ${sender_ID}

			|-'Only set the kill target for bots'
			/if (${Me.ID} != ${gl_assist_ID}) /varset gl_kill_ID ${target_ID} 

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

	/bc ${buff_target_ID}

	/declare i 	int local
	/declare ID int local -1

	/for i 1 to 100

		/if (${name.Find[${NearestSpawn[${i}]}]}) {
			/varset ID ${NearestSpawn[${i}].ID}
			/goto :FoundTarget
		}

	/next i

	:FoundTarget

	/if (${ID} != -1) {

		/t ${Spawn[ID ${ID}].Name} Buffs incoming...
		/bc buffs on ${ID}
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
	
	/declare x float local ${Me.X}
	/declare y float local ${Me.Y}

	/if (${Me.CurrentMana} < ${Spell[${spell_name}].Mana} * 1.3) {

		/bc Not enough mana to cast.

		/varset gl_spldly 20

		/interrupt
		/return 0
	}

	/if (${gl_is_following} && ${Spawn[ID ${gl_follow_ID}].Distance} > ${gl_lshdis}) {

		/bc Interrupting spell cast -- follow target needs me.

		/interrupt
		
		/if (${Me.Target.ID} == ${Me.ID}) //target clear

		/stick ID ${gl_follow_ID} ${gl_foldis}

		/varset gl_spldly 20

		/return 0
	}

	/call Memorize "${spell_name}" ${gem_number} 	
	/if (${Macro.Return}) /return 0

	/if (!${Me.SpellReady[${spell_name}]}) {

		/varset gl_spldly 20
		
		/return 0 
	}

	/bc Casting ${spell_name} on ${Spawn[ID ${target_ID}].Name}

	/casting "${spell_name}|Gem${gem_number}" -maxtries|3 -targetid|${target_ID}

	/delay 5

	:MonitorCasting

	/if (${Cast.Status.Count["C"]} > 0) {
				
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

			/interrupt
			/return 0
		}

		/if (${Me.X} != ${x} || ${Me.Y} != ${y}) {

			/bc I have moved from my casting position -- Interrupting.  (~X: )

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

		/varset gl_spldly 50
		/return 0
	}
}

sub Follow(int follow_ID) {

	/if (${follow_ID} == ${Me.ID}) {

		/varset gl_follow_ID 0
		/varset gl_is_following false
		/varset gl_agrtmr 12s

		/return
	}

	/if (${Spawn[ID ${follow_ID}].ID} && ${Spawn[ID ${follow_ID}].Distance} < ${gl_comdis}) {
		
		/varset gl_kill_ID 0
		/varset gl_isastg false

		/interrupt

		/attack off

		/varset gl_is_following true
		/varset gl_follow_ID ${follow_ID}
		/varset gl_follow_name ${Spawn[ID ${gl_follow_ID}].Name}		

		/stick ID ${gl_follow_ID} ${gl_foldis}
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
		
		/varset gl_spldly 15

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

		/echo Attempting to use ${FindItem[${item_name}].Name}
		
		/itemnotify ${FindItem[${item_name}].InvSlot} rightmouseup

		/varcalc sleep_time ${FindItem[${item_name}].CastTime} + 0.5

		/interrupt

		/delay ${sleep_time}
	
	} else {

		/echo Could not find an item matching '${item_name}'.
	}

	/return
}