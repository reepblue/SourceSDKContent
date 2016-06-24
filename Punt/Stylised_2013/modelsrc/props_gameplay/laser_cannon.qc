$staticprop
$modelname "props_gameplay/laser_cannon"
$scale "0.125000"
$body "Body" "laser_cannon_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "laser_cannon_ref"
$surfaceprop "no_decal"
$collisionmodel "laser_cannon_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
