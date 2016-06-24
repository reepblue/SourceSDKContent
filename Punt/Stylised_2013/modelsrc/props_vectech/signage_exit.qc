$staticprop
$modelname "props_vectech/signage_exit"
$scale "0.062500"
$body "Body" "signage_exit_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "signage_exit_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "signage_exit_off" }
	{ "signage_exit_on" }
}
$collisionmodel "signage_exit_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
$lod 35
{
	replacemodel "signage_exit_ref" "signage_exit_lod1"
}
