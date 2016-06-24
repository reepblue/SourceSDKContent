$staticprop
$modelname "props_vectech/cleanser"
$scale "0.125000"
$body "Body" "cleanser_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "cleanser_ref"
$surfaceprop "metal"
$texturegroup skinfamilies {
	{ "bluelight" }
	{ "greenlight" }
}
$collisionmodel "cleanser_phys"  {
	$concave
	$maxconvexpieces 1
	$automass
}
