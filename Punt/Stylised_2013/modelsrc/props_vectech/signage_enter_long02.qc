$staticprop
$modelname "props_vectech/signage_enter_long02"
$scale "0.062500"
$body "Body" "signage_enter_long02_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "signage_enter_long02_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "signage_enter_off" "signage_exit_arrow01" }
	{ "signage_enter_on" "signage_exit_arrow02" }
}
$collisionmodel "signage_enter_long02_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
