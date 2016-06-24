$staticprop
$modelname "props_vectech/signage_long02b"
$scale "0.062500"
$body "Body" "signage_long02b_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "signage_long02b_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "signage_exit_arrow04" "signage_box" }
	{ "signage_exit_arrow03" "signage_danger" }
}
$collisionmodel "signage_long02b_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
$lod 35
{
	replacemodel "signage_long02b_ref" "signage_long02b_lod1"
}
