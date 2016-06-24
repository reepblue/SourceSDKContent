$staticprop
$modelname "props_gameplay/laser_emitter_lower"
$scale "1.000000"
$body "Body" "laser_emitter_lower_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "laser_emitter_lower_ref"
$surfaceprop "metal"
$collisionmodel "laser_emitter_lower_phys"  {
	$concave
	$maxconvexpieces 4
	$automass
}
$attachment "laser_start" "" 0.000000 15.000000 -10.000000 absolute rigid rotate 0.000000 270.000000 0.000000
$attachment "beam2_1" "" -30.000000 15.000000 -10.000000 absolute rigid rotate 0.000000 0.000000 0.000000
$attachment "beam1_2" "" 0.000000 15.000000 -30.000000 absolute rigid rotate -90.000000 0.000000 0.000000
$attachment "beam2_2" "" 30.000000 15.000000 -10.000000 absolute rigid rotate 0.000000 180.000000 0.000000
$attachment "beam1_1" "" 0.000000 15.000000 30.000000 absolute rigid rotate 90.000000 0.000000 0.000000
