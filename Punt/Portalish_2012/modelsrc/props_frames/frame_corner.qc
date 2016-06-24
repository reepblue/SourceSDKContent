$staticprop
$modelname "props_frames/frame_corner"
$scale "1.000000"
$body "Body" "frame_corner_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "frame_corner_ref"
$surfaceprop "metal"
$collisionmodel "frame_corner_phys"  {
	$concave
	$maxconvexpieces 4
	$automass
}
