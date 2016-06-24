$staticprop
$modelname "props_gameplay/chamberdoor_lft"
$scale "0.125000"
$body "Body" "chamberdoor_lft_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "chamberdoor_lft_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "chamberdoor_lft_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
