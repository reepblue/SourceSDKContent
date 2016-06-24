$staticprop
$modelname "props_gameplay/cleanser_end"
$scale "1.000000"
$body "Body" "cleanser_end_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "cleanser_end_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "frame_light" }
	{ "frame_light_on" }
	{ "frame_light_time" }
}
$collisionmodel "cleanser_end_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
