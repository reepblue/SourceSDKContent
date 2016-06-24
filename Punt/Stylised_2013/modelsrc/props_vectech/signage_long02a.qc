$staticprop
$modelname "props_vectech/signage_long02a"
$scale "0.062500"
$body "Body" "signage_long02a_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "signage_long02a_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "signage_box" "signage_exit_arrow04" }
	{ "signage_danger" "signage_exit_arrow03" }
}
$collisionmodel "signage_long02a_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
$lod 35
{
	replacemodel "signage_long02a_ref" "signage_long02a_lod1"
}
