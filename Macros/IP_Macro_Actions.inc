#include IP_Macro_Commands.inc

sub Check_Actions() {

  /declare dbg bool local false

  /if (${dbg}) /echo assist
  /call Check_Assist
  /if (${dbg}) /echo follow
  /call Check_Follow
  /if (${dbg}) /echo med
  /call Check_Med
  /if (${dbg}) /echo combat
  /call Check_Combat
  /if (${dbg}) /echo loot
  /call Check_Loot
  /if (${dbg}) /echo yes
  /call Check_Yes
  /if (${dbg}) /echo zone
  /call Check_Zone

  /if (${Me.Moving} || ${gl_spldly} || ${Me.Invis} && !${gl_incmbt}) {
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
  /if (${dbg}) /echo pheals

  /if (${dbg}) /echo aoe
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

|-'Assist algorithm for physical damage dealers'
sub Check_Assist() {
  
  |-'Check for kill target'
  /if (${gl_kill_ID}) { 

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

        /if (!${Stick.Active} || ${Stick.StickTarget} != ${gl_kill_ID}) {

          /declare stick_distance float local 0.0

          /varcalc stick_distance ${Spawn[ID ${gl_kill_ID}].MaxRangeTo} - 1.0

          /stick ID ${gl_kill_ID} ${stick_distance} moveback ${gl_assdir}
        }

        |-'Enable attack'
        /if (!${NetBots[${Me.Name}].Attacking}) /attack on

        /declare abl  string  local 
        /declare i    int   local

        |-'Do all ready abilities'
        /for i 1 to ${gl_nmabls}

          /varset abl ${gl_cbtabs[${i}]}

          /if (${Me.AbilityReady[${abl}]}) /doability ${abl}

        /next i
      }
    }
  }

  /if (${Target.ID}) {
    |-'End combat'
    /if (!${Spawn[ID ${gl_kill_ID}].ID} || ${Spawn[ID ${gl_kill_ID}].Type.Equal["Corpse"]}) {

      /varset gl_kill_ID    0
      /varset gl_assist_ID  0
      /varset gl_isastg     false

      /if (${gl_isastg}) {
        
        /attack off
        /stick off
      }
    }

  } else {

    |-'Check if no one is being assisted'
    /if (!${gl_assist_ID}) {

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
  
  /if (!${gl_isastg}) {

    /if (${gl_is_following}) {

      /if (${Spawn[ID ${gl_follow_ID}].ID}) {

        /if (${Spawn[ID ${gl_follow_ID}].Distance} < ${gl_comdis}) {

          /if (!${Stick.Active} || ${Stick.StickTarget} != ${gl_follow_ID}) {

            /if (${Spawn[ID ${gl_follow_ID}].Distance} > ${gl_lshdis}) {
              
              /if (${Me.ID} == ${Target.ID}) //target clear

              /stick ID ${gl_follow_ID} ${gl_foldis}
            }
          }
          
        } else {

          /varset gl_is_following false
          /varset gl_follow_ID 0

          /stick off
        }

      } else {

        /varset gl_is_following false
        /varset gl_follow_ID 0

        /stick off
      }
    } else /if (${gl_follow_name.NotEqual["0"]}) {
      
      /if (${Spawn[${gl_follow_name}].ID} && ${Spawn[${gl_follow_name}].Distance} < ${gl_comdis}) {

        /bc Follow target acquired.
        /call Follow ${Spawn[${gl_follow_name}].ID}
      }
    }
  }
  
  /return
}

sub Check_Med() {
  
  /if (${gl_ismedg}) {
    
    /if (${gl_dmgtmr}) {

      /if (${Me.Sitting}) /stand

      /return
    }

    /if (${Stick.Stopped} || !${Stick.Active}) {

      /if (!${Me.Sitting} && !${Me.Moving}) {

        /if (!${Corpse.Open} && ${Me.SpellReady[1]}) {

          /sit 
        }
      }

    } else /if (!${Me.Standing}) {

      /stand
    } 
  }

  /return
}

sub Check_Combat() {

  /declare cncbts   string  local   ${NetBots.Client}
  /declare netbot   string  local   0
  /declare i      int   local

  /for i 1 to ${NetBots.Counts} 
    
    /varset netbot ${cncbts.Arg[${i}]}

    /if (${Spawn[PC ${netbot}].ID} && ${Spawn[PC ${netbot}].Distance} < ${gl_comdis}) {

      /if (${NetBots[${netbot}].CombatState} == 0) {

        /varset gl_cmbtmr 30s
        /varset gl_incmbt true

        /return
      } 
    }
  /next i

  /if (!${gl_cmbtmr}) /varset gl_incmbt false

  /return 
}

sub Check_Loot() {

  /if (${gl_rezflg}) {

    /call Event_Loot
    /varset gl_rezflg 0
  }

  /return
}

sub Check_Yes() {

  /if (${Window[ConfirmationDialogBox].Open}) {

    /notify ConfirmationDialogBox CD_Yes_Button leftmouseup
    /varset gl_rezflg 1
  }

  /return
}

sub Check_Zone() {
  
  /if (${gl_curzon.NotEqual[${Zone.Name}]}) {

    /varset gl_curzon     ${Zone.Name}
    /varset gl_kill_ID     0
    /varset gl_follow_ID  0
    /varset gl_incmbt     false
    /varset gl_isastg     false
    /varset m_autoing     false

    /stick off
    /attack off

    /if (${gl_iscstr}) /loadspells jim

    /if (${Me.Class.Name.Equal["Shaman"]} && ${FindItem["Guise of the Deceiver"].InvSlot}) {

      /itemnotify ${FindItem["Guise of the Deceiver"].InvSlot} rightmouseup
    }

    /if (${Me.Class.Name.Equal["Bard"]}) {

      /squelch /twist 1 2 3 4 5 
    }
  }

  /return
}

sub Check_Group_Heal() {
  
  /if (${Bool[${gl_ghlspl}]}) {

    /declare cncbts   string  local   ${NetBots.Client}
    /declare netbot   string  local   0
    /declare helcnt   int   local 0
    /declare i      int   local

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
    /declare i      int   local

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

    /declare cncbts   string  local   ${NetBots.Client}
    /declare netbot   string  local   0
    /declare lwstpc   int   local   100 
    /declare hltgid   int   local   
    /declare i      int   local

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
    /declare lwstpc   int   local   100 
    /declare hltgid   int   local   
    /declare i      int   local

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

sub Check_AoE() {

  /if (${gl_num_aoes} > 0 && ${gl_is_aoe_active}) {
    
    /declare aoe_spell  string  local
    /declare aoe_gem    string  local
    /declare i          int     local

    /for i 1 to ${gl_num_aoes} 

      /varset aoe_spell ${gl_aoes[${i},1]}
      /varset aoe_gem   ${gl_aoes[${i},2]}

      /call Cast ${aoe_spell} ${aoe_gem}

    /next i

    /return 1
  }

  /return 0
}

sub Check_Constant_Buffs() {

  /if (${gl_nmcsbf} > 0) {

    /declare bufspl string  local
    /declare bufgem string  local
    /declare buftgt string  local
    /declare buffid string  local
    /declare i      int     local

    /for i 1 to ${gl_nmcsbf}

      /varset bufspl ${gl_cstbfs[${i},1]}
      /varset bufgem ${gl_cstbfs[${i},2]}
      /varset buftgt ${gl_cstbfs[${i},3]}
      /varset buffid ${Spell[${bufspl}].ID}

      /if (${Spawn[PC ${buftgt}].ID} && ${Spawn[PC ${buftgt}].Distance} < ${gl_spldis}) {

        /if (${NetBots[${buftgt}].ID}) {
          
          /if (!${NetBots[${buftgt}].Buff.Find[${buffid}]}) {

            /call Cast ${bufspl} ${bufgem} ${Spawn[PC ${buftgt}].ID}

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

      /varset gl_pettid 0

      /pet taunt off

      /return ${Macro.Return}
    }
  }

  /return 0
}

sub Check_Pet_Buffs() {

  /if (${gl_nmptbf} > 0) {

    /declare bufspl string  local
    /declare bufgem string  local
    /declare buftgt string  local 
    /declare buffid string  local
    /declare i      int   local
    
    /for i 1 to ${gl_nmptbf}

      /varset bufspl ${gl_petbfs[${i},1]}
      /varset bufgem ${gl_petbfs[${i},2]}
      /varset buftgt ${gl_petbfs[${i},3]}
      /varset buffid ${Spell[${bufspl}].ID}

      /if (${NetBots[${buftgt}].PetID}) {
        
        /if (!${NetBots[${buftgt}].PetBuff.Find[${buffid}]}) {
        
          /call Cast ${bufspl} ${bufgem} ${NetBots[${buftgt}].PetID}

          /return ${Macro.Return}
        }

      } 

    /next i 
  }

  /return 0
}

sub Check_Cannibalize() {

  /if (${gl_iscstr} && ${gl_incmbt}) {

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

      /if (${Spawn[ID ${gl_kill_ID}].LineOfSight} && !${gl_dmgtmr}) {

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