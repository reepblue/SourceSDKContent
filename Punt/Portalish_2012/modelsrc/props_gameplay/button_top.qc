$staticprop
$modelname "props_gameplay/button_top"
$scale "1.000000"
$body "Body" "button_top_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "button_top_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "button_top-off" }
	{ "button_top-on" }
	{ "button_top-time" }
}
$collisionmodel "button_top_phys"  {
	$concave
	$maxconvexpieces 5
	$automass
}
