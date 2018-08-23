
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

			/bc Buffing ${Spawn[ID ${ID}].Name}...

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

			/bc Done buffing ${Spawn[ID ${ID}].Name}...
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

	/if (!${gl_combat_timer} && ${gl_is_following} && ${Spawn[ID ${gl_follow_id}].ID} && ${Spawn[ID ${gl_follow_id}].Distance} > ${gl_lshdis}) {

		/bc Interrupting spell cast -- follow target needs me.
		/tar ID ${gl_follow_id}
		/stick
		/squelch /target clear
		/interrupt
		/return 0
	}

	/call Memorize "${spell_name}" ${gem_number} 	

	:Wait
	
	/if (!${Me.SpellReady[${spell_name}]}) {

		/if (${waitcount} < 100) {
			
			/varcalc waitcount ${waitcount} + 1
			/delay 1
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

		/if (!${gl_combat_timer} && ${gl_is_following} && ${Spawn[ID ${gl_follow_id}].ID} && ${Spawn[ID ${gl_follow_id}].Distance} > ${gl_lshdis}) {

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

sub Follow(int follow_id) {

	|-'Never follow yourself'
	/if (${follow_id} == ${Me.ID}) {

		/varset gl_follow_name 	"0"
		/varset gl_follow_id 		0
		/varset gl_is_following false
		/varset gl_kill_ID      0 
		/varset gl_assist_ID    0 		

		/squelch /stick off

		/return
	}

	|-'Proximity check'
	/if (${Spawn[ID ${follow_id}].ID} && ${Spawn[ID ${follow_id}].Distance} < ${gl_comdis}) {
		
		|-'Do not follow corpses'
		/if (!${Spawn[ID ${follow_id}].Type.Equal["corpse"]}) {

			/varset gl_kill_ID 		0
			/varset gl_assist_ID 	0

			/interrupt

			/attack off

			/varset gl_is_following true
			/varset gl_follow_id    ${follow_id}
			/varset gl_follow_name  ${Spawn[ID ${gl_follow_id}].Name}		

			/stick ID ${gl_follow_id} ${gl_foldis}
		}
	}

	/return
}

sub Memorize(string spell_name, int gem_number) {

	/if (${Me.Gem[${spell_name}]} != ${gem_number}) {

		/bc Memorizing ${spell_name} to slot ${gem_number}.

		/memspell ${gem_number} "${spell_name}"

		/declare timeout int local 0

		:Memorizing

		/if (${Me.Gem[${spell_name}]} != ${gem_number} && ${timeout} < 150) {

			/delay 5
			/varcalc timeout ${timeout} + 5
			/goto :Memorizing
		}
		
		/stand

		/return 1
	}

	/return 0
}

sub Speak(int id, string msg) {

	/if (${Spawn[ID ${id}].ID} && ${Spawn[ID ${id}].Distance} < ${gl_comdis}) {

		/declare timeout int local 0

		:MoveToTarget

		/if (${Spawn[ID ${id}].Distance} > ${gl_spkdis}) {

			/moveto ID ${id}
			/delay 5
			/varcalc timeout ${timeout} + 5

			/if (${timeout} > 50) {
				/bc 'I could not reach my speak target'
				/stick off
				/goto :Break
			}

			/goto :MoveToTarget
		}

		/tar ID ${id}
		
		/delay 1

		/hail
		/say ${msg}	
	}

	:Break

	/return
}

sub Stop() {

	/varset gl_follow_name  "0"
	/varset gl_follow_id 		0
	/varset gl_is_following false
	/varset gl_kill_ID      0 
	/varset gl_assist_ID    0 

	
	/squelch /stick off

	/return
}

sub Toggle_Meditate() {

	/if (${gl_shdmed}) {

		/if (${gl_is_medding}) {

			/stand
		
			/varset gl_is_medding false

			/bc I am no longer meditating...

		} else {

			/varset gl_is_medding true

			/bc I am meditating...
		}
	}

	/return
}

sub Use_Item(string item_name) {
	
  /if (${FindItem[${item_name}].Name.NotEqual["NULL"]}) {

  	/if (${FindItem[${item_name}].EffectType.Find["click"]} == 1) {

  		/if (${FindItem[${item_name}].Timer} == 0) {
  		
  		  /stick off
  		  /interrupt

				:Casting
				/itemnotify ${FindItem[${item_name}].InvSlot} rightmouseup
				/delay 5
				
				/if (${FindItem[${item_name}].CastTime} > 0) {

					/delay 1

					/if (${Me.Casting.ID}) /goto :Casting

				} 
			} else {

  			/echo '${item_name} is not ready.'
  		}

  	} else {

  		/echo '${item_name} does not have a clickable effect.'
  	}

  } else {

		/echo 'Could not find an item matching ${item_name}'
	}

	/return 0
}