$staticprop
$modelname "props_vectech/chamber_doorframe"
$scale "0.125000"
$body "Body" "chamber_doorframe_ref"
$cdmaterials "models/props_vectech"
$sequence idle "chamber_doorframe_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "chamber_doorframe_phys"  {
	$concave
	$maxconvexpieces 3
	$automass
}
