sub Init_Bard() {
	
	/call Write_Control "behind" "true"	"false" "true" "false" "false" "false"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Nuke "NULL" "NULL" "1"
	/call Write_Debuff "NULL" "NULL" "1"

	/return
}

sub Init_Beastlord() {
	
	/call Write_Control "behind" "true" "false" "true" "false" "false" "true"
	/call Write_Ability "Kick" "1"
	/call Write_Target_Heal "NULL" "NULL" "NULL"
	/call Write_Nuke "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Berserker() {
	
	/call Write_Control "behind" "true" "false" "false" "false" "false" "false"
	/call Write_Ability "Frenzy" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Cleric() {
	
	/call Write_Control "behind" "false" "true" "true" "false" "false" "true"	
	/call Write_Group_Heal "NULL" "NULL" "NULL" "NULL"
	/call Write_Complete_Heal "NULL" "NULL" "NULL"
	/call Write_Target_Heal "Minor Healing" "1" "90"
	/call Write_Heal_Over_Time "NULL" "NULL" "NULL"
	/call Write_Nuke "NULL" "NULL" "1"
	/call Write_AoE "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Druid() {
	
	/call Write_Control "behind" "false" "true" "true" "false" "false" "true"	
	/call Write_Complete_Heal "NULL" "NULL" "NULL"
	/call Write_Target_Heal "Minor Healing" "1" "90"
	/call Write_Heal_Over_Time "NULL" "NULL" "NULL"
	/call Write_Nuke "Burst of Flame" "2" "1"
	/call Write_AoE "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Enchanter() {
	
	/call Write_Control "behind" "false" "false" "true" "false" "false" "true"
	/call Write_Nuke "Shallow Breath" "1" "1"
	/call Write_AoE "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Magician() {
	
	/call Write_Control "behind" "false" "false" "true" "false" "false" "true"
	/call Write_Nuke "Burst of Flame" "1" "1"
	/call Write_AoE "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Monk() {
	
	/call Write_Control "behind" "true" "false" "false" "false" "false" "false"
	/call Write_Ability "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Necromancer() {
	
	/call Write_Control "behind" "false" "false" "true" "false" "false" "true"
	/call Write_Nuke "Lifetap" "1" "1"
	/call Write_AoE "NULL" "NULL" "1"
	/call Write_Pet "Cavorting Bones" "8"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Paladin() {
	
	/call Write_Control "front" "true" "false" "true" "true" "false" "false"
	/call Write_Ability "Taunt" "1"
	/call Write_Ability "Bash" "2"	
	/call Write_Group_Heal "NULL" "NULL" "NULL" "NULL"
	/call Write_Target_Heal "NULL" "NULL" "NULL"
	/call Write_Heal_Over_Time "NULL" "NULL" "NULL"
	/call Write_Nuke "NULL" "NULL" "1"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Ranger() {
	
	/call Write_Control "behind" "true" "false" "true" "false" "false" "true"
	/call Write_Ability "Kick" "1"
	/call Write_Target_Heal "NULL" "NULL" "NULL"
	/call Write_Nuke "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Rogue() {
	
	/call Write_Control "behind" "true" "false" "false" "false" "false" "false"
	/call Write_Ability "Backstab" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Shadow_Knight() {
	
	/call Write_Control "front" "true" "false" "true" "true" "false" "false"	
	/call Write_Ability "Taunt" "1"
	/call Write_Ability "Bash" "2"
	/call Write_Nuke "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Shaman() {
	
	/call Write_Control "behind" "false" "true" "true" "false" "false" "true"
	/call Write_Target_Heal "Minor Healing" "1" "90"
	/call Write_Heal_Over_Time "NULL" "NULL" "NULL"
	/call Write_Nuke "Burst of Flame" "2" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Cannibalize "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Warrior() {
	
	/call Write_Control "front" "true" "false" "false" "true" "false" "false"
	/call Write_Ability "Kick" "1"
	/call Write_Ability "Taunt" "2"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Init_Wizard() {
	
	/call Write_Control "behind" "false" "false" "true" "false" "false" "true"
	/call Write_Nuke "Blast of Cold" "1" "1"
	/call Write_AoE "NULL" "NULL" "1"
	/call Write_Pet "NULL" "NULL"
	/call Write_Debuff "NULL" "NULL" "1"
	/call Write_Constant_Buff "NULL" "NULL" "NULL" "1"
	/call Write_Command_Buff "NULL" "NULL" "1"
	/call Write_Pet_Buff "NULL" "NULL" "NULL" "1"

	/return
}

sub Write_Control(string dir, string melee, string heal, string cast, string tank, string shoot, string med) {

	/call Write_Value "Control" "Assist_Direction" "${dir}"
	/call Write_Value "Control" "Should_Melee" "${melee}"
	/call Write_Value "Control" "Should_Heal" "${heal}"
	/call Write_Value "Control" "Should_Cast" "${cast}"
	/call Write_Value "Control" "Should_Tank" "${tank}"
	/call Write_Value "Control" "Should_Shoot" "${shoot}"
	/call Write_Value "Control" "Should_Meditate" "${med}"
	
	/return
}

sub Write_Ability(string abil, string i) {

	/call Write_Value "Abilities" "Ability#${i}" "${abil}"
	
	/return
}

sub Write_Group_Heal(string name, string gem, string perc, string numtargets) {

	/call Write_Value "Group_Heal" "Group_Heal_Spell" "${name}"
	/call Write_Value "Group_Heal" "Group_Heal_Gem" "${gem}"
	/call Write_Value "Group_Heal" "Group_Heal_Percent" "${perc}"
	/call Write_Value "Group_Heal" "Group_Heal_Targets" "${numtargets}"
	
	/return
}

sub Write_Complete_Heal(string name, string gem, string perc) {

	/call Write_Value "Complete_Heal" "Complete_Heal_Spell" "${name}"
	/call Write_Value "Complete_Heal" "Complete_Heal_Gem" "${gem}"
	/call Write_Value "Complete_Heal" "Complete_Heal_Percent" "${perc}"
	
	/return
}

sub Write_Target_Heal(string name, string gem, string perc) {
	
	/call Write_Value "Target_Heal" "Target_Heal_Spell" "${name}"
	/call Write_Value "Target_Heal" "Target_Heal_Gem" "${gem}"
	/call Write_Value "Target_Heal" "Target_Heal_Percent" "${perc}"
	
	/return
}

sub Write_Heal_Over_Time(string name, string gem, string perc) {

	/call Write_Value "Heal_Over_Time" "Heal_Over_Time_Spell" "${name}"
	/call Write_Value "Heal_Over_Time" "Heal_Over_Time_Gem" "${gem}"
	/call Write_Value "Heal_Over_Time" "Heal_Over_Time_Percent" "${perc}"
	
	/return
}

sub Write_Nuke(string name, string gem, string i) {

	/call Write_Value "Nukes" "Nuke_Spell#${i}" "${name}"
	/call Write_Value "Nukes" "Nuke_Gem#${i}" "${gem}"
	
	/return
}

sub Write_AoE(string name, string gem, string i) {

	/call Write_Value "Area_of_Effect" "AoE_Spell#${i}" "${name}"
	/call Write_Value "Area_of_Effect" "AoE_Gem#${i}" "${gem}"
	
	/return
}

sub Write_Pet(string name, string gem) {

	/call Write_Value "Pet" "Pet_Spell" "${name}"
	/call Write_Value "Pet" "Pet_Gem" "${gem}"
	
	/return
}

sub Write_Cannibalize(string name, string gem) {

	/call Write_Value "Cannibalize" "Cannibalize_Spell" "${name}"
	/call Write_Value "Cannibalize" "Cannibalize_Gem" "${gem}"

	/return
}

sub Write_Debuff(string name, string gem, string i) {

	/call Write_Value "Debuffs" "Debuff_Spell#${i}" "${name}"
	/call Write_Value "Debuffs" "Debuff_Gem#${i}" "${gem}"
	
	/return
}

sub Write_Constant_Buff(string name, string gem, string target, string i) {

	/call Write_Value "Constant_Buffs" "Constant_Buff_Spell#${i}" "${name}"
	/call Write_Value "Constant_Buffs" "Constant_Buff_Gem#${i}" "${gem}"
	/call Write_Value "Constant_Buffs" "Constant_Buff_Target#${i}" "${target}"
	
	/return
}

sub Write_Command_Buff(string name, string gem, string i) {

	/call Write_Value "Command_Buffs" "Command_Buff_Spell#${i}" "${name}"
	/call Write_Value "Command_Buffs" "Command_Buff_Gem#${i}" "${gem}"
	
	/return
}

sub Write_Pet_Buff(string name, string gem, string owner, string i) {

	/call Write_Value "Pet_Buffs" "Pet_Buff_Spell#${i}" "${name}"
	/call Write_Value "Pet_Buffs" "Pet_Buff_Gem#${i}" "${gem}"
	/call Write_Value "Pet_Buffs" "Pet_Buff_Owner#${i}" "${owner}"

	/return
}

sub Write_Value(string category, string key, string val) {

	/echo ${category} ${key} ${val}

	/if (${val.Equal["NULL"]}) /varset val ""

	/ini ${gl_bot_file} ${category} ${key} "${val}"	

	/return
}
