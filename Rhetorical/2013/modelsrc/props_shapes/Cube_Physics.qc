//$cd "G:\Source\SDK_rHetorical\sp\rhetorical\modelsrc\props_shapes"
$modelname "props_shapes/cube_physics.mdl"
$model "Body" "Cube_Physics"

// Here is our materials.
$cdmaterials "models\props_shapes/"

//Hbox
$hboxset "default"

//What material is it?
$surfaceprop "default"

// animations
$sequence idle "idle" ACT_IDLE 1 fps 12.00

// Physics
$collisionmodel "Cube_Physics_physmodel"  {

//	$concave
//	$maxconvexpieces 114
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