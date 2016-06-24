$staticprop
$modelname "props_vectech/chamber_buttonfloor_base"
$scale "0.125000"
$body "Body" "chamber_buttonfloor_base_ref"
$cdmaterials "models/props_vectech"
$sequence idle "chamber_buttonfloor_base_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "indicator_off" }
	{ "indicator_on" }
}
$collisionmodel "chamber_buttonfloor_base_phys"  {
	$concave
	$maxconvexpieces 5
	$automass
}
