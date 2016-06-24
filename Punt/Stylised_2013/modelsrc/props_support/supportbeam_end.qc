$staticprop
$modelname "supportbeam_end"
$scale "0.250000"
$body "Body" "supportbeam_end_ref"
$cdmaterials "models/props_support"
$sequence idle "supportbeam_end_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "support_sheet" }
	{ "support_sheet_skin001" }
	{ "support_sheet_skin002" }
}
$collisionmodel "supportbeam_end_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
