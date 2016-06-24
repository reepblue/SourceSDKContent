$staticprop
$modelname "props_gameplay/laser_receiver"
$scale "0.125000"
$body "Body" "laser_receiver_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "laser_receiver_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "laser_receiver_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
