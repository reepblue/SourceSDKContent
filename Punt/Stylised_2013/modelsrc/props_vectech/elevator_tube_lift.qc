$staticprop
$modelname "elevator_tube_lift"
$scale "0.250000"
$body "Body" "elevator_tube_lift_ref"
$cdmaterials "models/props_vectech"
$sequence idle "elevator_tube_lift_ref"
$surfaceprop "no_decal"
$collisionmodel "elevator_tube_lift_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
