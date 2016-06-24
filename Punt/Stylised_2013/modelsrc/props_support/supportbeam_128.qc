$staticprop
$modelname "supportbeam_128"
$scale "0.250000"
$body "Body" "supportbeam_128_ref"
$cdmaterials "models/props_support"
$sequence idle "supportbeam_128_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "support_sheet" }
	{ "support_sheet_skin001" }
	{ "support_sheet_skin002" }
}
$collisionmodel "supportbeam_128_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
