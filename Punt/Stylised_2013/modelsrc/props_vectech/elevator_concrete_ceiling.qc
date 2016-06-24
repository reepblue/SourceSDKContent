$staticprop
$modelname "props_vectech/elevator_concrete_ceiling"
$scale "1.000000"
$body "Body" "elevator_concrete_ceiling_ref"
$cdmaterials "models/props_vectech"
$sequence idle "elevator_concrete_ceiling_ref"
$surfaceprop "no_decal"
$collisionmodel "elevator_concrete_ceiling_phys"  {
	$concave
	$maxconvexpieces 8
	$automass
}
