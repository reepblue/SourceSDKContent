$staticprop
$modelname "props_gameplay/chamberdoor_frame"
$scale "1.000000"
$body "Body" "chamberdoor_frame_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "chamberdoor_frame_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "frame_light" }
	{ "frame_light_on" }
	{ "frame_light_time" }
}
$collisionmodel "chamberdoor_frame_phys"  {
	$concave
	$maxconvexpieces 3
	$automass
}
$lod 512
{
	replacemodel "chamberdoor_frame_ref" "chamberdoor_frame_lod1"
}
