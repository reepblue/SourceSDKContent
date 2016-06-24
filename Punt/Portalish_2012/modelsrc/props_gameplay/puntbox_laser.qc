$staticprop
$modelname "props_gameplaypuntbox_laser"
$scale "0.075000"
$body "Body" "puntbox_laser_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "puntbox_laser_ref"
$surfaceprop "2Tech_Box"
$texturegroup skinfamilies {
	{ "puntbox_laser" }
	{ "puntbox_laser_skin001" }
	{ "puntbox_laser_skin002" }
}
$collisionmodel "puntbox_laser_phys"  {
	$concave
	$maxconvexpieces 1
	$mass 40.000000
}

$keyvalues {
	"prop_data" {
		base		Metal.Small
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
$attachment "attach_laser_emit" "" 0.000000 -260.000000 0.000000 absolute rigid rotate 0.000000 90.000000 0.000000
