$staticprop
$modelname "props_gameplay/use_button"
$scale "0.090000"
$body "Body" "use_button_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "use_button_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "frame_light" }
	{ "frame_light_on" }
	{ "frame_light_time" }
}
$collisionmodel "use_button_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
