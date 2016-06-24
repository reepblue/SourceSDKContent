$staticprop
$modelname "props_signage/punt_panel_arrowdown"
$scale "0.062500"
$body "Body" "punt_panel_arrowdown_ref"
$cdmaterials "models/props_signage"
$sequence idle "punt_panel_arrowdown_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "arrow_signage_off" }
	{ "arrow_signage_on" }
	{ "arrow_signage_time" }
}
$collisionmodel "punt_panel_arrowdown_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
