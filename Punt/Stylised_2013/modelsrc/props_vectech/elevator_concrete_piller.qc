$staticprop
$modelname "props_vectech/elevator_concrete_piller"
$scale "1.000000"
$body "Body" "elevator_concrete_piller_ref"
$cdmaterials "models/props_vectech"
$sequence idle "elevator_concrete_piller_ref"
$surfaceprop "no_decal"
$collisionmodel "elevator_concrete_piller_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
