$staticprop
$modelname "props_signage/indicator_panel"
$scale "0.062500"
$body "Body" "indicator_panel_ref"
$cdmaterials "models/props_signage"
$sequence idle "indicator_panel_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "door_signage_off" }
	{ "door_signage_on" }
	{ "door_signage_time" }
}
$collisionmodel "indicator_panel_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
