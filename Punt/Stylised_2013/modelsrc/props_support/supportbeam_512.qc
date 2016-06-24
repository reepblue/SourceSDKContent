$staticprop
$modelname "props_support/supportbeam_512"
$scale "0.250000"
$body "Body" "supportbeam_512_ref"
$cdmaterials "models/props_support"
$sequence idle "supportbeam_512_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "support_sheet" }
	{ "support_sheet_skin001" }
	{ "support_sheet_skin002" }
}
$collisionmodel "supportbeam_512_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
