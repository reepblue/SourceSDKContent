//$cd "G:\Source\rHetorical\rhetorical_asw\modelsrc\props_skybox\clouddome"
$modelname "props_skybox/clouddome.mdl"
$scale "1.00"
$model "Body" "clouddome_new.smd"

// Here is our materials.
$cdmaterials "models\props_skybox/"

$texturegroup skinfamilies
{
	{ "clouddome.vmt" }
	{ "clouddome_skin001.vmt" }
}

//Hbox
$hboxset "default"

//What material is it?
$surfaceprop "default"

// animations
$sequence idle "idle" ACT_IDLE 1 fps 12.00

