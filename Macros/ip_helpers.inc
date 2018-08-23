
sub Range_Check(int target_id, float range) {
  
  |-'Proximity check'
  /if (${Spawn[ID ${target_id}].ID} && ${Spawn[ID ${target_id}].Distance} < ${range}) {

    /if (!${Spawn[ID ${target_id}].Type.Equal["corpse"]}) {

      return 1
    }
  }

  /return 0
}
