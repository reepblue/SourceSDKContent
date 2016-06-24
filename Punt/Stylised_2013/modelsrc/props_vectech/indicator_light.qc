$staticprop
$modelname "props_vectech/indicator_light"
$scale "0.072500"
$body "Body" "indicator_light_ref"
$cdmaterials "models/props_vectech"
$sequence idle "indicator_light_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "indicator_light_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
