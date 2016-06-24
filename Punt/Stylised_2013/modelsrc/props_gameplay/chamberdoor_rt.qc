$staticprop
$modelname "props_gameplay/chamberdoor_rt"
$scale "0.125000"
$body "Body" "chamberdoor_rt_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "chamberdoor_rt_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "chamberdoor_rt_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
