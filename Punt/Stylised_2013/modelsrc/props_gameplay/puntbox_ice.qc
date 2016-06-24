$staticprop
$modelname "props_gameplay/puntbox_ice"
$scale "0.075000"
$body "Body" "puntbox_ice_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "puntbox_ice_ref"
$surfaceprop "2Tech_IceBox"
$texturegroup skinfamilies {
	{ "puntbox_ice" }
	{ "puntbox_ice_skin001" }
	{ "puntbox_ice_skin002" }
	{ "puntbox_ice_skin003" }
	{ "puntbox_ice_skin004" }
	{ "puntbox_ice_skin005" }
}
$collisionmodel "puntbox_ice_phys"  {
	$concave
	$maxconvexpieces 1
	$mass 40.000000
}
$lod 65
{
	replacemodel "puntbox_ice_ref" "puntbox_ice_lod1"
}

$keyvalues {
	"prop_data" {
		base		Metal.Small
		allowstatic	true
		breakable_model	MetalChunks
		breakable_count	5
		breakable_skin	0
	}
}
