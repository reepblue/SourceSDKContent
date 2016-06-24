$staticprop
$modelname "props_gameplay/puntbox"
$scale "0.155000"
$body "Body" "puntbox_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "puntbox_ref"
$surfaceprop "C:/Program Files (x86)/Steam/steamapps/reepblue/sourcesdk_content/punt/modelsrc/props_gameplay"
$texturegroup skinfamilies {
	{ "frame_light" }
	{ "frame_light_on" }
	{ "frame_light_time" }
}
$collisionmodel "puntbox_phys"  {
	$concave
	$maxconvexpieces 1
	$mass 1.000000
}

$keyvalues {
	"prop_data" {
		base		Item.Small
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
