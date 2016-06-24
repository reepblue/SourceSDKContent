$staticprop
$modelname "props_vectech/indicator_panel"
$scale "0.062500"
$body "Body" "indicator_panel_ref"
$cdmaterials "models/props_vectech"
$sequence idle "indicator_panel_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "indicator_off" }
	{ "indicator_on" }
}
$collisionmodel "indicator_panel_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
