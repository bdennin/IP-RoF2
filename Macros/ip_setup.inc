
sub setup_macro() {

  /call setup_variables
  /call setup_class
  /call check_plugins

  /squelch /filter mq on
  /squelch /tgb on
  /squelch /hidecorpse all
  /squelch /hidecorpse none
  /squelch /hidecorpse looted
  /squelch /bccmd channel notes
  /squelch /stick load

  /attack off

  /return
}

sub setup_variables() {

  /if (!${Defined[gl_follow_name]})   /declare gl_follow_name   string  global "0"
  /if (!${Defined[gl_follow_id]})     /declare gl_follow_id     int     global 0
  /if (!${Defined[gl_kill_id]})       /declare gl_kill_id       int     global 0
  /if (!${Defined[gl_assist_id]})     /declare gl_assist_id     int     global 0
  /if (!${Defined[gl_is_following]})  /declare gl_is_following  bool    global false
  /if (!${Defined[gl_is_medding]})    /declare gl_is_medding    bool    global false

  /declare gl_settings_file   string  outer   "Settings\Settings.ini"
  /declare gl_bot_file        string  outer   "Bots\${MacroQuest.Server}_${Me.Class.ShortName}_${Me.Name}.ini"
  /declare gl_window_file     string  outer   "Data\Window_Names.ini"
  /declare gl_current_zone    string  outer   ""
  /declare gl_gate_item       string  outer   ""

  /declare gl_combat_timer        timer   outer 0s
  /declare gl_damage_taken_timer  timer   outer 0s
  /declare gl_spldly  timer   outer   0s
  /declare gl_petdly  timer   outer   0s

  /declare gl_comdis  float   outer 0.0
  /declare gl_rngdis  float   outer 0.0
  /declare gl_spldis  float   outer 0.0
  /declare gl_ghldis  float   outer 0.0
  /declare gl_lshdis  float   outer 0.0
  /declare gl_foldis  float   outer 0.0
  /declare gl_spkdis  float   outer 0.0

  /declare gl_engpct  int   outer   0
  /declare gl_tgtpct  int   outer   0
  
  /declare gl_rezflg        bool outer false
  /declare gl_helpet        bool outer true
  /declare gl_is_aoe_active bool outer false
  /declare gl_heloth        bool outer true


  /if (!${Ini[${gl_settings_file}].Length}) {

    /ini ${gl_settings_file} "Assists"  "Engage_Percent"            "97"

    /ini ${gl_settings_file} "Spells"     "Gate_Item_Name"          "Boots of the Nexus"

    /ini ${gl_settings_file} "Distances"  "Communication_Distance"  "2000.0"
    /ini ${gl_settings_file} "Distances"  "Ranged_Attack_Distance"  "125.0"
    /ini ${gl_settings_file} "Distances"  "Spell_Casting_Distance"  "100.0"
    /ini ${gl_settings_file} "Distances"  "Group_Heal_Distance"     "50.0"
    /ini ${gl_settings_file} "Distances"  "Follow_Leash_Distance"   "20.0"
    /ini ${gl_settings_file} "Distances"  "Follow_Distance"         "10.0"
    /ini ${gl_settings_file} "Distances"  "Speak_Distance"          "5.0"
  }

  /varset gl_engpct    ${Ini[${gl_settings_file},"Assists","Engage_Percent"]}
  /varset gl_gate_item "${Ini[${gl_settings_file},"Spells","Gate_Item_Name"]}"

  /varset gl_comdis ${Ini[${gl_settings_file},"Distances","Communication_Distance"]}
  /varset gl_rngdis ${Ini[${gl_settings_file},"Distances","Ranged_Attack_Distance"]}
  /varset gl_spldis ${Ini[${gl_settings_file},"Distances","Spell_Casting_Distance"]}
  /varset gl_ghldis ${Ini[${gl_settings_file},"Distances","Group_Heal_Distance"]}
  /varset gl_lshdis ${Ini[${gl_settings_file},"Distances","Follow_Leash_Distance"]}
  /varset gl_foldis ${Ini[${gl_settings_file},"Distances","Follow_Distance"]}
  /varset gl_spkdis ${Ini[${gl_settings_file},"Distances","Speak_Distance"]}

  /return
}

sub setup_class() {
  
  /declare gl_assdir    string  outer
  /declare gl_is_melee  bool    outer
  /declare gl_ishelr    bool    outer
  /declare gl_iscstr    bool    outer
  /declare gl_istank    bool    outer
  /declare gl_shdsht    bool    outer
  /declare gl_shdmed    bool    outer
  
  /declare gl_combat_abilities[6] string  outer
  /declare gl_num_abilities    int outer
  
  /declare gl_ghlspl    string  outer
  /declare gl_ghlgem    string  outer
  /declare gl_ghlpct    int   outer
  /declare gl_nmghtg    int   outer

  /declare gl_chlspl    string  outer
  /declare gl_chlgem    string  outer
  /declare gl_chlpct    int     outer   

  /declare gl_target_heal_spell_name    string  outer
  /declare gl_thlgem    string  outer
  /declare gl_target_heal_percent    int   outer   

  /declare gl_hotspl    string  outer   
  /declare gl_hotgem    string  outer
  /declare gl_hotpct    int   outer

  /declare gl_petspl    string  outer
  /declare gl_petgem    string  outer

  /declare gl_canni_spell string outer
  /declare gl_canni_gem   int    outer
 
  /declare gl_nukes[4,3]        string outer
  /declare gl_aoes[5,3]         string outer
  /declare gl_debuffs[4,3]      string outer
  /declare gl_const_buffs[8,3]  string outer
  /declare gl_comm_buffs[4,3]   string outer
  /declare gl_pet_buffs[8,3]    string outer
  
  /declare gl_num_nukes       int outer
  /declare gl_num_aoes        int outer
  /declare gl_num_debuffs     int outer
  /declare gl_num_const_buffs int outer
  /declare gl_num_comm_buffs  int outer
  /declare gl_num_pet_buffs   int outer
  

  /if (!${Ini[${gl_bot_file}].Length}) {
    
    /echo "First time setup for ${Me.Class.Name} on ${MacroQuest.Server}."

    /if (${Me.Class.Name.Equal["Shadow Knight"]}) {

      /call Init_Shadow_Knight
    
    } else {

      /call Init_${Me.Class.Name}
    }
  }

  /declare iter   string  local
  /declare i      int     local

  /varset gl_assdir ${Ini[${gl_bot_file},"Control","Assist_Direction"]}
  /varset gl_is_melee ${Ini[${gl_bot_file},"Control","Should_Melee"]}
  /varset gl_ishelr ${Ini[${gl_bot_file},"Control","Should_Heal"]}
  /varset gl_iscstr ${Ini[${gl_bot_file},"Control","Should_Cast"]}
  /varset gl_istank ${Ini[${gl_bot_file},"Control","Should_Tank"]}
  /varset gl_shdsht ${Ini[${gl_bot_file},"Control","Should_Shoot"]}
  /varset gl_shdmed ${Ini[${gl_bot_file},"Control","Should_Meditate"]}

  /for i 1 to 6
    
    /varset iter "${Ini[${gl_bot_file},"Abilities","Ability#${i}"]}"
    
    /if (${Bool[${iter}]}) {

      /varset gl_combat_abilities[${i}] ${iter}

      /varset gl_num_abilities ${i}
    }

  /next i
  
  /varset iter "${Ini[${gl_bot_file},"Group_Heal","Group_Heal_Spell"]}"
      
  /if (${Bool[${iter}]}) {
    /if (${Me.Book[${iter}]}) {
      
      /varset gl_ghlspl "${Ini[${gl_bot_file},"Group_Heal","Group_Heal_Spell"]}"
      /varset gl_ghlgem "${Ini[${gl_bot_file},"Group_Heal","Group_Heal_Gem"]}"
      /varset gl_ghlpct ${Ini[${gl_bot_file},"Group_Heal","Group_Heal_Percent"]}
      /varset gl_nmghtg ${Ini[${gl_bot_file},"Group_Heal","Group_Heal_Targets"]}
    } else {    

      /echo "${iter} not found in book. Ignoring..."
    }
  }

  /varset iter "${Ini[${gl_bot_file},"Complete_Heal","Complete_Heal_Spell"]}"    
  
  /if (${Bool[${iter}]}) {
    /if (${Me.Book[${iter}]}) {
  
      /varset gl_chlspl "${Ini[${gl_bot_file},"Complete_Heal","Complete_Heal_Spell"]}"
      /varset gl_chlgem "${Ini[${gl_bot_file},"Complete_Heal","Complete_Heal_Gem"]}"
      /varset gl_chlpct ${Ini[${gl_bot_file},"Complete_Heal","Complete_Heal_Percent"]}
    } else {

      /echo "${iter} not found in book. Ignoring..."
    }
  }
  
  /varset iter "${Ini[${gl_bot_file},"Target_Heal","Target_Heal_Spell"]}"
  
  /if (${Bool[${iter}]}) {
    /if (${Me.Book[${iter}]}) {
  
      /varset gl_target_heal_spell_name "${Ini[${gl_bot_file},"Target_Heal","Target_Heal_Spell"]}"
      /varset gl_thlgem "${Ini[${gl_bot_file},"Target_Heal","Target_Heal_Gem"]}"
      /varset gl_target_heal_percent ${Ini[${gl_bot_file},"Target_Heal","Target_Heal_Percent"]}
    } else {    

      /echo "${iter} not found in book. Ignoring..."
    }
  }

  /varset iter "${Ini[${gl_bot_file},"Heal_Over_Time","Heal_Over_Time_Spell"]}"
      
  /if (${Bool[${iter}]}) {
    /if (${Me.Book[${iter}]}) {
  
      /varset gl_hotspl "${Ini[${gl_bot_file},"Heal_Over_Time","Heal_Over_Time_Spell"]}"
      /varset gl_hotgem "${Ini[${gl_bot_file},"Heal_Over_Time","Heal_Over_Time_Gem"]}"
      /varset gl_hotpct ${Ini[${gl_bot_file},"Heal_Over_Time","Heal_Over_Time_Percent"]}
    } else {    

      /echo "${iter} not found in book. Ignoring..."
    }
  }


  /varset iter "${Ini[${gl_bot_file},"Pet","Pet_Spell"]}"

  /if (${Bool[${iter}]}) {
    /if (${Me.Book[${iter}]}) {
      
      /varset gl_petspl "${Ini[${gl_bot_file},"Pet","Pet_Spell"]}"
      /varset gl_petgem "${Ini[${gl_bot_file},"Pet","Pet_Gem"]}"
    } else {    

      /echo "${iter} not found in book. Ignoring..."
    }
  }

  /varset iter "${Ini[${gl_bot_file},"Cannibalize","Cannibalize_Spell"]}"

  /if (${Bool[${iter}]}) {
    /if (${Me.Book[${iter}]}) {
  
      /varset gl_canni_spell "${Ini[${gl_bot_file},"Cannibalize","Cannibalize_Spell"]}"
      /varset gl_canni_gem ${Ini[${gl_bot_file},"Cannibalize","Cannibalize_Gem"]}
    } else {    

      /echo "${iter} not found in book. Ignoring..."
    }
  }

  /for i 1 to 4
    
    /varset iter "${Ini[${gl_bot_file},"Nukes","Nuke_Spell#${i}"]}"
   
    /if (${Bool[${iter}]}) {
      /if (${Me.Book[${iter}]}) {

        /varset gl_nukes[${i},1] "${Ini[${gl_bot_file},"Nukes","Nuke_Spell#${i}"]}"
        /varset gl_nukes[${i},2] "${Ini[${gl_bot_file},"Nukes","Nuke_Gem#${i}"]}"
    
        /varset gl_num_nukes ${i}
      } else {

        /echo "${iter} not found in book. Ignoring..."
      }
    }
  /next i

  /for i 1 to 4
    
    /varset iter "${Ini[${gl_bot_file},"Debuffs","Debuff_Spell#${i}"]}"
       
    /if (${Bool[${iter}]}) {
      /if (${Me.Book[${iter}]}) {

        /varset gl_debuffs[${i},1] "${Ini[${gl_bot_file},"Debuffs","Debuff_Spell#${i}"]}"
        /varset gl_debuffs[${i},2] "${Ini[${gl_bot_file},"Debuffs","Debuff_Gem#${i}"]}"
    
        /varset gl_num_debuffs ${i}
      } else {

        /echo "${iter} not found in book. Ignoring..."
      }
    }
  /next i

  /for i 1 to 8
    
    /varset iter "${Ini[${gl_bot_file},"Constant_Buffs","Constant_Buff_Spell#${i}"]}"
   
    /if (${Bool[${iter}]}) {
      /if (${Me.Book[${iter}]}) {

        /varset gl_const_buffs[${i},1] "${Ini[${gl_bot_file},"Constant_Buffs","Constant_Buff_Spell#${i}"]}"
        /varset gl_const_buffs[${i},2] "${Ini[${gl_bot_file},"Constant_Buffs","Constant_Buff_Gem#${i}"]}"
        /varset gl_const_buffs[${i},3] "${Ini[${gl_bot_file},"Constant_Buffs","Constant_Buff_Target#${i}"]}"

        /varset gl_num_const_buffs ${i}
      } else {

        /echo "${iter} not found in book. Ignoring..."
      }
    }
  /next i

  /for i 1 to 4
    
    /varset iter "${Ini[${gl_bot_file},"Command_Buffs","Command_Buff_Spell#${i}"]}"
    
    /if (${Bool[${iter}]}) {
      /if (${Me.Book[${iter}]}) {

        /varset gl_comm_buffs[${i},1] "${Ini[${gl_bot_file},"Command_Buffs","Command_Buff_Spell#${i}"]}"
        /varset gl_comm_buffs[${i},2] "${Ini[${gl_bot_file},"Command_Buffs","Command_Buff_Gem#${i}"]}"

        /varset gl_num_comm_buffs ${i}
      } else {

        /echo "${iter} not found in book. Ignoring..."
      }
    }
  /next i

  /for i 1 to 4
    
    /varset iter "${Ini[${gl_bot_file},"Pet_Buffs","Pet_Buff_Spell#${i}"]}"
       
    /if (${Bool[${iter}]}) {
      /if (${Me.Book[${iter}]}) {

        /varset gl_pet_buffs[${i},1] "${Ini[${gl_bot_file},"Pet_Buffs","Pet_Buff_Spell#${i}"]}"
        /varset gl_pet_buffs[${i},2] "${Ini[${gl_bot_file},"Pet_Buffs","Pet_Buff_Gem#${i}"]}"
        /varset gl_pet_buffs[${i},3] "${Ini[${gl_bot_file},"Pet_Buffs","Pet_Buff_Owner#${i}"]}"

        /varset gl_num_pet_buffs ${i}
      } else {

        /echo "${iter} not found in book. Ignoring..."
      }
    }
  /next i
  
  /for i 1 to 4
       
    /if (${Bool[${iter}]}) {
      /varset iter "${Ini[${gl_bot_file},"Area_of_Effect","AoE_Spell#${i}"]}"
      
      /if (${Me.Book[${iter}]}) {

        /varset gl_aoes[${i},1] "${Ini[${gl_bot_file},"Area_of_Effect","AoE_Spell#${i}"]}"
        /varset gl_aoes[${i},2] "${Ini[${gl_bot_file},"Area_of_Effect","AoE_Gem#${i}"]}"

        /varset gl_num_aoes ${i}
      } else {

        /echo "${iter} not found in book. Ignoring..."
      }
    }
  /next i

  /return
}

sub check_plugins() {
  
  /if (!${Plugin[MQ2EQBC].Name.Length}) {

    /echo Plugin MQ2EQBC is not loaded, attempting to resolve...
  
    /plugin MQ2EQBC
    /delay 10s !${Select[${EQBC},NULL]}
  
    /if (${Select[${EQBC},NULL]}) /endmacro
  }

  /if (!${EQBC.Connected}) {

    /bccmd connect
    /delay 10s ${EQBC.Connected}

    /if (!${EQBC.Connected}) /endmacro
  }
  
  /if (!${Plugin[MQ2NetBots].Name.Length}) {

    /echo Plugin MQ2NetBots is not loaded, attempting to resolve...
    
    /plugin MQ2NetBots
    /delay 3s ${NetBots}
  
    /if (!${NetBots}) /endmacro
  }
  
  /if (${NetBots}) {
    /squelch /netbots on grab=on send=on
  }
  
  /if (!${Plugin[MQ2Exchange].Name.Length}) {
  
    /echo Plugin MQ2Exchange is not loaded, attempting to resolve...
    
    /plugin MQ2Exchange
    /delay 3s ${Plugin[MQ2Exchange].Name.Length}
  
    /if (!${Plugin[MQ2Exchange].Name.Length}) /endmacro
  }

  /if (!${Plugin[MQ2MoveUtils].Name.Length}) {
    
    /echo Plugin MQ2MoveUtils is not loaded, attempting to resolve...
    
    /plugin MQ2MoveUtils
    /delay 3s !${Select[${Stick.Status},NULL]}
    
    /if (${Select[${Stick.Status},NULL]}) /endmacro
  }

  /if (${Select[${Me.Class.ShortName},BRD]}) {
    
    /if (!${Plugin[MQ2Twist].Name.Length}) {
      
      /echo Plugin Unable to detect MQ2Twist, attempting to resolve...
      
      /plugin MQ2Twist
      /delay 3s !${Select[${Twist},NULL]}
    } 
  }

  /if (!${Plugin[MQ2ItemDisplay].Name.Length}) {
    
    /echo Plugin Unable to detect MQ2ItemDisplay, attempting to resolve...
    
    /plugin MQ2ItemDisplay
    /delay 3s ${Plugin[MQ2ItemDisplay].Name.Length}
  }

  /if (!${Plugin[MQ2HUD].Name.Length}) {
    
    /echo Plugin Unable to detect MQ2HUD, attempting to resolve...
    
    /plugin MQ2HUD
    /delay 2s ${Plugin[MQ2HUD].Name.Length}
  }

  /if (!${Plugin[MQ2Cast].Name.Length}) {
    
    /echo Plugin MQ2Cast is not loaded, attempting to resolve...
    
    /plugin MQ2Cast
    /delay 3s ${Cast.Active}
    
    /if (!${Cast.Active}) /endmacro
  }

  /return
}
