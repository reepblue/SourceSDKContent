//$cd "G:\Source\SDK_rHetorical\sp\rhetorical\modelsrc\props_shapes"
$modelname "props_shapes/torus.mdl"
$scale "1.00"
$model "Body" "torus.smd"

// Here is our materials.
$cdmaterials "models\props_shapes/"

//Hbox
$hboxset "default"

//What material is it?
$surfaceprop "default"

// animations
$sequence idle "idle" ACT_IDLE 1 fps 12.00

// Attachments
$attachment "attachment" "root" 0 0 48 absolute

