$staticprop
$modelname "elevator_tube_entrance"
$scale "0.250000"
$body "Body" "elevator_tube_entrance_ref"
$cdmaterials "models/props_vectech"
$sequence idle "elevator_tube_entrance_ref"
$surfaceprop "no_decal"
$collisionmodel "elevator_tube_entrance_phys"  {
	$concave
	$maxconvexpieces 110
	$automass
}
