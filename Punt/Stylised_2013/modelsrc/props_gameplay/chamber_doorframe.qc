$staticprop
$modelname "props_gameplay/chamber_doorframe"
$scale "0.125000"
$body "Body" "chamber_doorframe_ref"
$cdmaterials "models/props_gameplay"
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
$lod 45
{
	replacemodel "chamber_doorframe_ref" "chamber_doorframe_lod1"
}
