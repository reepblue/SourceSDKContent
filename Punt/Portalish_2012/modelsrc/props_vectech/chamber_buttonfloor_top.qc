$staticprop
$modelname "props_vectech/chamber_buttonfloor_top"
$scale "0.125000"
$body "Body" "chamber_buttonfloor_top_ref"
$cdmaterials "models/props_vectech"
$sequence idle "chamber_buttonfloor_top_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "chamber_buttonfloor_top_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
