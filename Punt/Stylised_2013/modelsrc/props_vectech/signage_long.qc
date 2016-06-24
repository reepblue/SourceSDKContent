$staticprop
$modelname "props_vectech/signage_long"
$scale "0.062500"
$body "Body" "signage_long_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "signage_long_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "signage_box" }
	{ "signage_danger" }
}
$collisionmodel "signage_long_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
$lod 35
{
	replacemodel "signage_long_ref" "signage_long_lod1"
}
