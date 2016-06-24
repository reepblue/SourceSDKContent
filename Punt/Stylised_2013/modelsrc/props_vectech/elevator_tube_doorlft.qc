$staticprop
$modelname "props_vectech/elevator_tube_doorlft"
$scale "0.250000"
$body "Body" "elevator_tube_doorlft_ref"
$cdmaterials "models/props_vectech"
$sequence idle "elevator_tube_doorlft_ref"
$surfaceprop "no_decal"
$collisionmodel "elevator_tube_doorlft_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
