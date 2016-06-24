$staticprop
$modelname "props_gameplay/puntbox"
$scale "0.075000"
$body "Body" "puntbox_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "puntbox_ref"
$surfaceprop "2Tech_Box"
$texturegroup skinfamilies {
	{ "puntbox" }
	{ "puntbox_skin001" }
	{ "puntbox_skin002" }
	{ "puntbox_skin003" }
	{ "puntbox_skin004" }
	{ "puntbox_skin005" }
}
$collisionmodel "puntbox_phys"  {
	$concave
	$maxconvexpieces 54
	$mass 40.000000
}

$lod 45
{
	replacemodel "puntbox_ref" "puntbox_lod1"
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