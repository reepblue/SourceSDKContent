$staticprop
$modelname "props_gameplay/chamber_buttonfloor_top"
$scale "0.125000"
$body "Body" "chamber_buttonfloor_top_ref"
$cdmaterials "models/props_gameplay"
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
$lod 45
{
	replacemodel "chamber_buttonfloor_top_ref" "chamber_buttonfloor_top_lod1"
}
