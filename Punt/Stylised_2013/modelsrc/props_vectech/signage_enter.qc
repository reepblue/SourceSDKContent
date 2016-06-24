$staticprop
$modelname "props_vectech/signage_enter"
$scale "0.062500"
$body "Body" "signage_enter_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "signage_enter_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "signage_enter_off" }
	{ "signage_enter_on" }
}
$collisionmodel "signage_enter_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
$lod 35
{
	replacemodel "signage_enter_ref" "signage_enter_lod1"
}
