$staticprop
$modelname "props_vectech/vent_wall"
$scale "0.125000"
$body "Body" "vent_wall_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "vent_wall_ref"
$surfaceprop "metal"
$collisionmodel "vent_wall_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
