$staticprop
$modelname "props_vectech/signage_exit_long02"
$scale "0.062500"
$body "Body" "signage_exit_long02_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "signage_exit_long02_ref"
$surfaceprop "no_decal"
$texturegroup skinfamilies {
	{ "signage_exit_off" "signage_exit_arrow01" }
	{ "signage_exit_on" "signage_exit_arrow02" }
}
$collisionmodel "signage_exit_long02_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
