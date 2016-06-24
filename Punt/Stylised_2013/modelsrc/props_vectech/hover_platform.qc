$staticprop
$modelname "props_gameplay/hover_platform"
$scale "0.125000"
$body "Body" "hover_platform_ref"
$cdmaterials "models/props_vectech"
$sequence idle "hover_platform_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "hover_platform_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
