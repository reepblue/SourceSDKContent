$staticprop
$modelname "props_gameplay/laser_receiver"
$scale "1.000000"
$body "Body" "laser_receiver_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "laser_receiver_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "light_off" "frame_light" }
	{ "light_on" "frame_light_on" }
	{ "light_time" "frame_light_time" }
}
$collisionmodel "laser_receiver_phys"  {
	$concave
	$maxconvexpieces 5
	$automass
}
$attachment "laser_target" "" 0.000000 12.000000 -6.000000 absolute rigid rotate 0.000000 270.000000 0.000000
