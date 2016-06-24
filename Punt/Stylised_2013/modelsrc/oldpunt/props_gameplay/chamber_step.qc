$staticprop
$modelname "props_gameplay/chamber_step"
$scale "0.250000"
$body "Body" "chamber_step_ref"
$cdmaterials "models/props_gameplay"
$sequence idle "chamber_step_ref"
$surfaceprop "metal"
$collisionmodel "chamber_step_phys"  {
	$concave
	$maxconvexpieces 4
	$automass
}
$lod 512
{
	replacemodel "chamber_step_ref" "chamber_step_lod1"
}
