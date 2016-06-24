$staticprop
$modelname "props_vectech/elevator_concrete_curve"
$scale "1.000000"
$body "Body" "elevator_concrete_curve_ref"
$cdmaterials "models/props_vectech"
$sequence idle "elevator_concrete_curve_ref"
$surfaceprop "no_decal"
$collisionmodel "elevator_concrete_curve_phys"  {
	$concave
	$maxconvexpieces 2
	$automass
}
