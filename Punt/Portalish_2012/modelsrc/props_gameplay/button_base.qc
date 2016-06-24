$staticprop
$modelname "props_gameplay/button_base"
$scale "1.000000"
$body "Body" "button_base_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "button_base_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "frame_light" }
	{ "frame_light_on" }
	{ "frame_light_time" }
}
$collisionmodel "button_base_phys"  {
	$concave
	$maxconvexpieces 9
	$automass
}
