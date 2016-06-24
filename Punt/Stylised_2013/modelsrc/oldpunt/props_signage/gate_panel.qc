$staticprop
$modelname "props_signage/gate_panel"
$scale "0.062500"
$body "Body" "gate_panel_ref"
$cdmaterials "models/props_signage"
$sequence idle "gate_panel_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "gate_signage_blank" }
	{ "gate_signage_and" }
	{ "gate_signage_or" }
}
$collisionmodel "gate_panel_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
