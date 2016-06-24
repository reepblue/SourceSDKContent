$staticprop
$modelname "props_gameplay/laser_box"
$scale "0.155000"
$body "Body" "laser_box_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "laser_box_ref"
$surfaceprop "C:/Program Files (x86)/Steam/steamapps/reepblue/sourcesdk_content/punt/modelsrc/props_gameplay"
$collisionmodel "laser_box_phys"  {
	$concave
	$maxconvexpieces 5
	$mass 40.000000
}

$keyvalues {
	"prop_data" {
		base		2Tech_Box
		health		0
		allowstatic	true
		breakable_model	MetalChunks
		breakable_count	5
		breakable_skin	0
	}
	physgun_interactions
	{
		preferred_carryangles "0.000000 0.000000 0.000000"
		onlaunch 		default
	}
}
$attachment "attach_laser_emit" "" 0.000000 -120.000000 0.000000 absolute rigid rotate 0.000000 90.000000 0.000000


