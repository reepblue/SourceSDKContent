//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: Here are tags to enable/disable things throughout the source code.
//
//=============================================================================//

#ifndef SDK_SHAREDDEFS_H
#define SDK_SHAREDDEFS_H
#ifdef _WIN32
#pragma once
#endif

#ifdef SWARM_DLL
// Quick swap! NUM_AI_CLASSES = LAST_SHARED_ENTITY_CLASS in ASW!
#define NUM_AI_CLASSES LAST_SHARED_ENTITY_CLASS
#endif

// For GetGameDescription()
#define GAMENAME "Template"


// Fell free to edit! You can comment these out or add more defines.  
//========================
// PLAYER RELATED OPTIONS
//========================

//-----Optional Defines-----
// PLAYER_HEALTH_REGEN : Regen the player's health much like it does in Portal/COD
// PLAYER_MOUSEOVER_HINTS : When the player has their crosshair over whatever we put in UpdateMouseoverHints() it will display a hint
// PLAYER_IGNORE_FALLDAMAGE : Ignore fall damage.
// PLAYER_DISABLE_THROWING : Disables throwing in the player pickup controller.
//------------------

// Use a navmesh? Comment if no.
//========================
// NAV_MESH
//========================
#ifndef SWARM_DLL
	#define USE_NAV_MESH
#endif

#endif // SDK_SHAREDDEFS_H