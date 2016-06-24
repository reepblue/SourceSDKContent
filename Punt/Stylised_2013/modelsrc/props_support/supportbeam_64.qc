$staticprop
$modelname "supportbeam_64"
$scale "0.250000"
$body "Body" "supportbeam_64_ref"
$cdmaterials "models/props_support"
$sequence idle "supportbeam_64_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "support_sheet" }
	{ "support_sheet_skin001" }
	{ "support_sheet_skin002" }
}
$collisionmodel "supportbeam_64_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
