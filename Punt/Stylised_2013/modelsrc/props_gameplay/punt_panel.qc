$staticprop
$modelname "props_gameplay/punt_panel"
$scale "0.062500"
$body "Body" "punt_panel_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "punt_panel_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "target_signage_off" }
	{ "target_signage_on" }
	{ "target_signage_time" }
}
$collisionmodel "punt_panel_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
$lod 45
{
	replacemodel "punt_panel_ref" "punt_panel_lod1"
}
