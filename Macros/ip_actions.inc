#include ip_commands.inc

sub handle_actions() {

  /declare dbg bool local false

  /if (${dbg}) /echo attack
  /call Check_Attack
  /if (${dbg}) /echo kill
  /call Check_Kill_Target
  /if (${dbg}) /echo assist
  /call Check_Auto_Assist
  /if (${dbg}) /echo follow
  /call Check_Follow
  /if (${dbg}) /echo med
  /call Check_Med
  /if (${dbg}) /echo zone
  /call Check_Zone

  /if (${Me.Moving} || ${gl_spldly} || ${Me.Invis}) {
    /return
  }

  /if (${dbg}) /echo gheal
  /call Check_Group_Heal
  /if (${Macro.Return}) /return
  /if (${dbg}) /echo cheal
  /call Check_Complete_Heal
  /if (${Macro.Return}) /return
  /if (${dbg}) /echo theal
  /call Check_Target_Heal
  /if (${Macro.Return}) /return
  /if (${dbg}) /echo hot
  /call Check_HoT
  /if (${Macro.Return}) /return
  /if (${dbg}) /echo hot
  /call Check_Heal_Group
  /if (${Macro.Return}) /return

  /if (${dbg}) /echo area
  /call Check_AoE
  /if (${Macro.Return}) /return

  /if (${dbg}) /echo chbuffs
  /call Check_Constant_Buffs
  /if (${Macro.Return}) /return
  
  /if (${dbg}) /echo pet
  /call Check_Pet_Summon
  /if (${Macro.Return}) /return
  /if (${dbg}) /echo petbuffs
  /call Check_Pet_Buffs
  /if (${Macro.Return}) /return

  /if (${dbg}) /echo canni1
  /call Check_Cannibalize
  /if (${Macro.Return}) /return
  
  /if (${dbg}) /echo nukes  
  /call Check_Nuke
  /if (${Macro.Return}) /return 
  
  /return
} 

sub Check_Attack() {

  |-'Check for assist target'
  /if (${gl_assist_ID}) { 

    |-'Melee routine'
    /if (${gl_is_melee}) {

      |-'Acquire target'
      /if (!${Target.ID} || ${Target.ID} != ${gl_kill_ID}) /tar ID ${gl_kill_ID}

      |-'Ranged attack'
      /if (${gl_shdsht}) {
    
        /if (!${Stick.Active} || ${Stick.StickTarget} != ${gl_kill_ID}) {

          /stick ID ${gl_kill_ID} moveback ${gl_rngdis}  
        }
      
        /if (${Me.RangedReady}) /ranged
    
      |-'Melee attack'
      } else {

        |-'Move to correct distance'
        /if (!${Stick.Active} || ${Stick.StickTarget} != ${gl_kill_ID}) {

          /declare stick_distance float local ${Spawn[ID ${gl_kill_ID}].MaxRangeTo}

          /if (${stick_distance} > 40) {
            /varcalc stick_distance ${stick_distance} * 0.33
          } else {
            /varcalc stick_distance ${stick_distance} * 0.60
          }
          
          /stick ID ${gl_kill_ID} ${stick_distance} moveback ${gl_assdir}
        }

        |-'Enable attack'
        /if (!${NetBots[${Me.Name}].Attacking}) /attack on

        /declare abl  string  local 
        /declare i    int     local

        |-'Do all ready abilities'
        /for i 1 to ${gl_num_abilities}

          /varset abl ${gl_combat_abilities[${i}]}

          /if (${Me.AbilityReady[${abl}]}) /doability ${abl}

        /next i
      }
    }
  }

  /return
}

sub Check_Kill_Target() {

  |-'Check for kill target'
  /if (${gl_kill_ID}) {

    |-'Clear kill target if it is dead or not found'
    /if (!${Spawn[ID ${gl_kill_ID}].ID} || ${Spawn[ID ${gl_kill_ID}].Type.Equal["Corpse"]}) {

      /if (${gl_assist_ID}) {
        
        /attack off
        /stick off
        /squelch /target clear
      }

      /varset gl_kill_ID    0
      /varset gl_assist_ID  0
    }
  } 

  /return
}

sub Check_Auto_Assist() {

  |-'Check for auto-assist on active character'
  /if (${NetBots[${Me.Name}].Attacking}) {

    /if (!${gl_kill_ID}) {

      /if (${Target.Type.Equal["NPC"]}) {

        /if (${Target.PctHPs} > 0 && ${Target.PctHPs} < ${gl_engpct}) {

          /bc assist on ${Target.ID}
        }
      } 
    }
  }

  /return
}

sub Check_Follow() {
  
  /if (!${gl_kill_ID}) {

    /if (${gl_is_following}) {

      /if (${Spawn[ID ${gl_follow_id}].ID} && ${Spawn[ID ${gl_follow_id}].Type.NotEqual["Corpse"]}) {

        /if (${Spawn[ID ${gl_follow_id}].Distance} < ${gl_comdis}) {
       
          /if (${Target.ID} != ${gl_follow_id}) /squelch /target clear

          /if (!${Stick.Active} || ${Stick.StickTarget} != ${gl_follow_id}) {

            /if (${Spawn[ID ${gl_follow_id}].Distance} > ${gl_lshdis}) {
              
              /stick ID ${gl_follow_id} ${gl_foldis}
            }
          }
          
        } else {

          /varset gl_is_following false
          /varset gl_follow_id 0

          /stick off
        }

      } else {

        /varset gl_is_following false
        /varset gl_follow_id 0

        /stick off
      }
    } else /if (${Bool[${gl_follow_name}]}) {

      /call Follow ${Spawn[${gl_follow_name}].ID}
    }
  }
  
  /return
}

sub Check_Med() {
  
  /if (${gl_is_medding}) {
    
    /if (${gl_damage_taken_timer}) {

      /if (${Me.Sitting}) /standdd

      /return
    }

    /if (!${gl_is_following} || ${Spawn[ID ${gl_follow_id}].Distance} <= ${gl_lshdis})  {

      /if (!${Me.Sitting}) {

        /if (!${Corpse.Open}) {

          /sit 
        }
      }

    } else /if (!${Me.Standing}) {

      /stand
    } 
  }

  /return
}

sub Check_Zone() {
  
  /if (${gl_current_zone.NotEqual[${Zone.Name}]}) {

    /varset gl_current_zone   ${Zone.Name}
    /varset gl_kill_ID        0
    /varset gl_assist_ID      0
    /varset gl_follow_id      0

    /stick off
    /attack off

    /if (${Me.Class.Name.Equal["Bard"]}) {

      /squelch /twist 1 2 3 4 5 
    }
  }

  /return
}

sub Check_Group_Heal() {
  
  /if (${Bool[${gl_ghlspl}]}) {

    /declare cncbts   string  local ${NetBots.Client}
    /declare netbot   string  local 0
    /declare helcnt   int     local 0
    /declare i        int     local

    /for i 1 to ${NetBots.Counts} 
      
      /varset netbot ${cncbts.Arg[${i}]}

      /if (${Spawn[PC ${netbot}].ID} && ${Spawn[PC ${netbot}].Distance} < ${gl_ghldis}) {

        /if (${NetBots[${netbot}].InGroup}) {
          
          /if (${NetBots[${netbot}].PctHPs} < ${gl_ghlpct}) {

            /varcalc helcnt ${helcnt} + 1
          }
        }
      }

    /next i

    /if (${helcnt} >= ${gl_nmghtg}) {

      /call Cast ${gl_ghlspl} ${gl_ghlgem} ${Me.ID}

      /return ${Macro.Return}
    }
  }

  /return 0
} 

sub Check_Complete_Heal() {

  /if (${Bool[${gl_chlspl}]}) {

    /declare cncbts   string  local   ${NetBots.Client}
    /declare netbot   string  local   0
    /declare lwstpc   int   local   100 
    /declare hltgid   int   local   
    /declare i        int   local

    /for i 1 to ${NetBots.Counts} 
      
      /varset netbot ${cncbts.Arg[${i}]}

      /if (${Spawn[PC ${netbot}].Class.Name.Equal["Warrior"]}) {
        
        /if (${Spawn[PC ${netbot}].ID} && ${Spawn[PC ${netbot}].Distance} < ${gl_spldis}) {

          /if (${NetBots[${netbot}].PctHPs} < ${lwstpc}) {

            /varset lwstpc ${NetBots[${netbot}].PctHPs}
            /varset hltgid ${NetBots[${netbot}].ID}
          }
        }
      }

    /next i

    /if (${lwstpc} < ${gl_chlpct}) {

      /call Cast ${gl_chlspl} ${gl_chlgem} ${hltgid}

      /return ${Macro.Return}
    }
  }

  /return 0
}

sub Check_Target_Heal() {

  /if (${Bool[${gl_thlspl}]}) {

    /declare cncbts string  local   ${NetBots.Client}
    /declare netbot string  local   0
    /declare lwstpc int     local   100 
    /declare hltgid int     local   
    /declare i      int     local

    /for i 1 to ${NetBots.Counts}     

      /varset netbot ${cncbts.Arg[${i}]}

      /if (${Spawn[PC ${netbot}].ID} && ${Spawn[PC ${netbot}].Distance} < ${gl_spldis}) {

        /if (${NetBots[${netbot}].PctHPs} < ${lwstpc}) {

          /varset lwstpc ${NetBots[${netbot}].PctHPs}
          /varset hltgid ${NetBots[${netbot}].ID}
        }
      }

    /next i

    /if (${Me.Pet.ID} && ${gl_helpet}) {

      /if (${Me.Pet.PctHPs} < ${lwstpc}) {

      /varset lwstpc ${Me.Pet.PctHPs}
      /varset hltgid ${Me.Pet.ID}
      }
    }

    /if (${lwstpc} < ${gl_thlpct}) {

      /call Cast ${gl_thlspl} ${gl_thlgem} ${hltgid}

      /return ${Macro.Return}
    }

  }

  /return 0
}

sub Check_HoT() {

  /if (${Bool[${gl_hotspl}]}) {

    /declare cncbts   string  local   ${NetBots.Client}
    /declare hotsid   string  local   ${Spell[${gl_hotspl}].ID}
    /declare netbot   string  local   0
    /declare lwstpc   int     local   100 
    /declare hltgid   int     local   
    /declare i        int     local

    /for i 1 to ${NetBots.Counts} 
      
      /varset netbot ${cncbts.Arg[${i}]}

      /if (${Spawn[PC ${netbot}].ID} && ${Spawn[PC ${netbot}].Distance} < ${gl_spldis}) {

        /if (${NetBots[${netbot}].PctHPs} < ${lwstpc}) {

          /if (!${NetBots[${netbot}].ShortBuff.Find[${hotsid}]}) {
        
            /varset lwstpc ${NetBots[${netbot}].PctHPs}
            /varset hltgid ${NetBots[${netbot}].ID}
          }
        }
      }

    /next i

    /if (${Me.Pet.ID} && ${gl_helpet}) {

      /if (${Me.Pet.PctHPs} < ${lwstpc}) {

        /varset lwstpc ${Me.Pet.PctHPs}
        /varset hltgid ${Me.Pet.ID}
      }
    }

    /if (${lwstpc} < ${gl_hotpct}) {

      /call Cast ${gl_hotspl} ${gl_hotgem} ${hltgid}

      /return ${Macro.Return}
    }
  }
  
  /return 0
}

sub Check_Heal_Group() {

  /if (${gl_heloth} && ${Bool[${gl_thlspl}]}) {

    /declare lwstpc int     local   100 
    /declare hltgid int     local   
    /declare i      int     local

    /for i 0 to ${Group} 
    
      /if (${Group.Member[${i}].PctHPs} != 0 && ${Group.Member[${i}].PctHPs} < ${lwstpc}) {
        /if (${Group.Member[${i}].ID} && ${Group.Member[${i}].Distance} < ${gl_spldis}) {
          /varset lwstpc ${Group.Member[${i}].PctHPs}
          /varset hltgid ${Group.Member[${i}].ID}
        }
      }
    /next i


    /if (${lwstpc} < ${gl_thlpct} && ${Me.CurrentMana} > ${Spell[${gl_thlspl}].Mana} * 1.3) {

      /bc Attemping to heal ${Spawn[${hltgid} ID].Name}
      /call Cast ${gl_thlspl} ${gl_thlgem} ${hltgid}

      /return ${Macro.Return}
    }
  }
  
  /return 0
}

sub Check_AoE() {

  /if (${gl_num_aoes} > 0 && ${gl_is_aoe_active}) {
    
    /declare aoe_spell  string  local
    /declare aoe_gem    string  local
    /declare i          int     local

    /for i 1 to ${gl_num_aoes} 

      /if (!${gl_is_aoe_active}) /return 0

      /varset aoe_spell ${gl_aoes[${i},1]}
      /varset aoe_gem   ${gl_aoes[${i},2]}

      /call Memorize ${aoe_spell} ${aoe_gem}

      /if (${Me.SpellReady[${aoe_spell}]}) {
        
        /call Cast ${aoe_spell} ${aoe_gem} ${Me.ID}
      }

    /next i

    /return 1

  }

  /return 0
}

sub Check_Constant_Buffs() {

  /if (${gl_num_const_buffs} > 0) {

    /declare buff_spell_name  string  local
    /declare buff_gem         string  local
    /declare buff_target      string  local
    /declare buff_spell_ID    string  local
    /declare i                int     local

    /for i 1 to ${gl_num_const_buffs}

      /varset buff_spell_name ${gl_const_buffs[${i},1]}
      /varset buff_gem        ${gl_const_buffs[${i},2]}
      /varset buff_target     ${gl_const_buffs[${i},3]}
      /varset buff_spell_ID   ${Spell[${buff_spell_name}].ID}

      /if (${Spawn[PC ${buff_target}].ID} && ${Spawn[PC ${buff_target}].Distance} < ${gl_spldis}) {

        /if (${NetBots[${buff_target}].ID}) {
          
          /if (!${NetBots[${buff_target}].Buff.Find[${buff_spell_ID}]}) {

            /call Cast ${buff_spell_name} ${buff_gem} ${Spawn[PC ${buff_target}].ID}

            /return ${Macro.Return}
          }
        }
      }

    /next i
  }

  /return 0
}

sub Check_Pet_Summon() {

  /if (${Bool[${gl_petspl}]}) {
    
    /if (!${Me.Pet.ID}) {

      /call Cast ${gl_petspl} ${gl_petgem} ${Me.ID}

      /squelch /pet taunt off

      /return ${Macro.Return}
    }
  }

  /return 0
}

sub Check_Pet_Buffs() {

  /if (${gl_num_pet_buffs} > 0) {

    /declare buff_spell_name  string  local
    /declare buff_gem         string  local
    /declare buff_target      string  local
    /declare buff_spell_ID    string  local
    /declare i                int     local

    /for i 1 to ${gl_num_pet_buffs}

      /varset buff_spell_name ${gl_pet_buffs[${i},1]}
      /varset buff_gem        ${gl_pet_buffs[${i},2]}
      /varset buff_target     ${gl_pet_buffs[${i},3]}
      /varset buff_spell_ID   ${Spell[${buff_spell_name}].ID}

      /if (${NetBots[${buff_target}].PetID}) {
        
        /if (!${NetBots[${buff_target}].PetBuff.Find[${buff_spell_ID}]}) {
        
          /call Cast ${buff_spell_name} ${buff_gem} ${NetBots[${buff_target}].PetID}

          /return ${Macro.Return}
        }

      } 

    /next i 
  }

  /return 0
}

sub Check_Cannibalize() {

  /if (${gl_iscstr} && ${gl_combat_timer}) {

    /if (${Me.PctMana} < 90 && ${Me.PctHPs} > 60) {
        
      /if (${Me.PctMana} < 60 && ${Me.AltAbilityReady["Cannibalization"]}) {

        /aa act cannibalization

        /delay 3s

        /return 1

      } else /if (${Bool[${gl_canni_spell}]}) {

        /call Cast ${gl_canni_spell} ${gl_cangem}

      } else /if (${FindItem["Manastone"].InvSlot}) {

        /call Use_Item "Manastone"
      }
    }
  }
    
  /return 0
}

sub Check_Nuke() {

  /if (${gl_kill_ID} != 0) {
        
    /if (${gl_num_nukes} > 0) {

      /if (${Spawn[ID ${gl_kill_ID}].LineOfSight} && !${gl_damage_taken_timer}) {

        /declare nuke_spell string  local
        /declare nuke_gem   string  local
        /declare i          int     local

        /for i 1 to ${gl_num_nukes}

          /varset nuke_spell  ${gl_nukes[${i},1]}
          /varset nuke_gem    ${gl_nukes[${i},2]}

          /call Cast ${nuke_spell} ${nuke_gem} ${gl_kill_ID}

        /next i
      }
    }
  }

  /return 0
}