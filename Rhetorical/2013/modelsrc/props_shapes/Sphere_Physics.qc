//$cd "G:\Source\SDK_rHetorical\sp\rhetorical\modelsrc\props_shapes"
$modelname "props_shapes/sphere_physics.mdl"
$scale "0.50"
$model "Body" "Sphere.smd"

// Here is our materials.
$cdmaterials "models\props_shapes/"

//Hbox
$hboxset "default"

//What material is it?
$surfaceprop "metal_bouncy"

// animations
$sequence idle "idle" ACT_IDLE 1 fps 12.00

// Physics
$collisionmodel "Sphere_Physics.smd" {

	$mass 40.0
	$inertia 1.00
	$damping 0.10
	$rotdamping 0.00
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

// Attachments
$attachment "attachment" "root" 0 0 48 absolute