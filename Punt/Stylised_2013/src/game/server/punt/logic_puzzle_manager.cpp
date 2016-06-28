//=========== Copyright © 2012, rHetorical, All rights reserved. =============
//
// Purpose: Punt Puzzle manager.
//		
//=============================================================================

#include "cbase.h"
#include <convar.h>
#include "func_break.h"
#include "engine/ienginesound.h"
#include "vphysics/player_controller.h"
#include "soundent.h"
#include "game.h"

//#include <stdio.h>
//#include <time.h>
//#include <sstream>

ConVar w_season( "w_season", "0", FCVAR_HIDDEN );
// 0 = Spring
// 1 = Summer
// 2 = Fall
// 3 = Winter

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CpointPuzzleManage : public CLogicalEntity
{
public:
	DECLARE_CLASS( CpointPuzzleManage, CLogicalEntity);

	void Precache( void );
	void Spawn( );
	void Think( void );
	void SaveThink( void );

	DECLARE_DATADESC();

	//Constructor
	CpointPuzzleManage ()
	{
		m_intseason = 0;
		m_bisSaveEnabled = false;
		m_bAllowAutoSave = false;
		m_bGun = false;
//		m_bCanLaunch = false;
		m_bAllowRecording = false;
	}

	void InputStartPuzzle( inputdata_t &inputData);
	void InputEndPuzzle( inputdata_t &inputData);
	void InputSaveProgress( inputdata_t &inputData);
	void InputGiveGun( inputdata_t &inputData);
//	void InputAllowLaunching( inputdata_t &inputData);
	void GunManage();
	void Record();
	void Report( inputdata_t &inputData );
	void ResetConvars();

private:

	bool	m_bisSaveEnabled;
	bool	m_bAllowAutoSave;
	bool	m_bGun;
	bool	m_bHasGun;
//	bool	m_bCanLaunch;
	bool	m_bAllowRecording;
	bool	m_bIsRecording;
	float	m_flSaveTimeAmount;
	int		m_intseason;

	COutputEvent	m_OnPuzzleStart;
	COutputEvent	m_OnPuzzleEnd;
};

LINK_ENTITY_TO_CLASS( logic_puzzle_manager, CpointPuzzleManage);

BEGIN_DATADESC( CpointPuzzleManage)

	//Save/load
	DEFINE_FIELD( m_bisSaveEnabled, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bAllowAutoSave, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flSaveTimeAmount, FIELD_FLOAT ),
	DEFINE_FIELD( m_bGun, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_bHasGun, FIELD_BOOLEAN ),
	DEFINE_THINKFUNC( SaveThink ), // Register new think function


	DEFINE_KEYFIELD( m_bisSaveEnabled, FIELD_BOOLEAN, "allowsave" ),
//	DEFINE_KEYFIELD( m_bCanLaunch, FIELD_BOOLEAN, "canlaunch" ),
	DEFINE_KEYFIELD( m_bAllowAutoSave, FIELD_BOOLEAN, "allowautosave" ),
	DEFINE_KEYFIELD( m_flSaveTimeAmount, FIELD_FLOAT, "autosavetime" ),
	DEFINE_KEYFIELD( m_bGun, FIELD_BOOLEAN, "gun" ),
	DEFINE_KEYFIELD( m_intseason, FIELD_INTEGER, "season" ),
	DEFINE_KEYFIELD( m_bAllowRecording, FIELD_BOOLEAN, "allowrecording" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "StartPuzzle", InputStartPuzzle ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EndPuzzle", InputEndPuzzle ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SaveProgress", InputSaveProgress ),
	DEFINE_INPUTFUNC( FIELD_VOID, "GivePuntgun", InputGiveGun ),
//	DEFINE_INPUTFUNC( FIELD_VOID, "UpgradePuntgun", InputAllowLaunching ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Report", Report ),

	DEFINE_OUTPUT( m_OnPuzzleStart, "OnPuzzleStart" ),
	DEFINE_OUTPUT( m_OnPuzzleEnd, "OnPuzzleEnd" ),

END_DATADESC()

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CpointPuzzleManage::Precache( void )
{
	//Precatching Sounds for those who can't precatche themselves.
	PrecacheScriptSound( "Punt.FilterActivate" );
	PrecacheScriptSound( "Punt.FilterDeactivate" );
	PrecacheScriptSound( "Punt.Filterloop" );
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CpointPuzzleManage::Spawn( void )
{
	Precache();
	BaseClass::Spawn();
	GunManage();
	ResetConvars();

	RegisterThinkContext( "SaveContext" );
	SetContextThink( &CpointPuzzleManage::SaveThink, gpGlobals->curtime, "SaveContext" );

	if ( !m_bGun )
	{
//		engine->ServerCommand("bind mouse2 +use\n");
		ConVar *player_has_puntgun = cvar->FindVar( "player_has_puntgun" );
		player_has_puntgun->SetValue(0);
	}
	else if (!m_bHasGun)
	{
//		engine->ServerCommand("bind e +use\n");
//		engine->ServerCommand("bind mouse2 +attack2\n");
		ConVar *player_has_puntgun = cvar->FindVar( "player_has_puntgun" );
		player_has_puntgun->SetValue(1);
		SetNextThink( gpGlobals->curtime + .10 );
	}

	if ( m_bAllowAutoSave )
	{
		SaveThink();
	}

	// seasons
	if ( m_intseason == 0)
	{
		w_season.SetValue(0);
	}

	if ( m_intseason == 1)
	{
		w_season.SetValue(1);
	}

	if ( m_intseason == 2)
	{
		w_season.SetValue(2);
	}

	if ( m_intseason == 3)
	{
		w_season.SetValue(3);
	}


	m_bIsRecording = false;

}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CpointPuzzleManage::Think( void )
{
	BaseClass::Precache();
	engine->ServerCommand("give_puntgun\n");
	Msg ("=======================Trying to GivePlayerPuntgun\n");
	Msg ("Resetting Convars\n");
	ConVar *player_has_puntgun = cvar->FindVar( "player_has_puntgun" );
	player_has_puntgun->SetValue(1);

//	if (m_bCanLaunch == false)
//	{
//		ConVar *puntgun_allow_launching = cvar->FindVar( "puntgun_allow_launching" );
//		puntgun_allow_launching->SetValue(0);
//	}

	ResetConvars();
	m_bHasGun = true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CpointPuzzleManage::SaveThink( void )
{
	SetNextThink(gpGlobals->curtime + m_flSaveTimeAmount, "SaveContext" );
	if (m_bAllowAutoSave)
	{
		engine->ServerCommand( "autosave\n" );
	}
} 

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CpointPuzzleManage::InputStartPuzzle( inputdata_t &inputData )
{
	m_OnPuzzleStart.FireOutput( inputData.pActivator, this );
	if ( m_bAllowRecording && !m_bIsRecording )
	{
		Record();
		m_bIsRecording = true;
	}

	if (m_bisSaveEnabled)
	{
		engine->ClearSaveDir();
		engine->ServerCommand( "autosave\n" );
	}
}

void CpointPuzzleManage::InputEndPuzzle( inputdata_t &inputData )
{
	m_OnPuzzleEnd.FireOutput( inputData.pActivator, this );
	if ( m_bAllowRecording && m_bIsRecording )
	{
		engine->ServerCommand( "stop\n" );
		Msg ("Stop Demo\n");
		m_bIsRecording = false;
	}

	if (m_bisSaveEnabled)
	{
		engine->ClearSaveDir();
		engine->ServerCommand( "autosave\n" );
	}
}

void CpointPuzzleManage::InputSaveProgress( inputdata_t &inputData )
{
		engine->ClearSaveDir();
		engine->ServerCommand( "autosave\n" );
}

void CpointPuzzleManage::InputGiveGun ( inputdata_t &inputData )
{
		engine->ServerCommand("give_puntgun\n");
		ConVar *player_has_puntgun = cvar->FindVar( "player_has_puntgun" );
		player_has_puntgun->SetValue(1);
		Msg ("Resetting Convars\n");
		ResetConvars();
		m_bHasGun = true;
}

void CpointPuzzleManage::GunManage()
{
//	if (m_bCanLaunch)
//	{
//		ConVar *puntgun_allow_launching = cvar->FindVar( "puntgun_allow_launching" );
//		puntgun_allow_launching->SetValue(1);
//	}

}
//void CpointPuzzleManage::InputAllowLaunching ( inputdata_t &inputData )
//{
//	m_bCanLaunch = true;
//	GunManage();
//}

//-----------------------------------------------------------------------------
// Purpose: To record Demos
//-----------------------------------------------------------------------------
void CpointPuzzleManage::Record()
{
	if ( m_bAllowRecording )
	{
//		TODO: MAKE THE FILES RECORD BASED ON TIME! time (NULL);
		engine->ServerCommand(  "record puzzlemanager_replay_\n" );
		Msg ("Recording Demo\n");
		m_bIsRecording = true;
		return;
	}

	if (m_bAllowRecording && !m_bIsRecording)
	{
		Error ("Demo failed to record\n");
	}

}

//-----------------------------------------------------------------------------
// Purpose: To report Bool states to user.
//-----------------------------------------------------------------------------
void CpointPuzzleManage::Report( inputdata_t &inputData )
{
	// Check AutoSaving
	if (m_bAllowAutoSave)
	{
		Msg ("Autosaving: on\n");
	}
	else
	{
		Msg ("Autosaving: off\n");
	}
	// Check Save Enabled
	if (m_bisSaveEnabled)
	{
		Msg ("Manager Saving: on\n");
	}
	else
	{
		Msg ("Manager Saving: off\n");
	}
	// Check to see if the manager gave the player the gun
	if (m_bHasGun)
	{
		Msg ("Player has Puntgun given: on\n");
	}
	else
	{
		Msg ("Player has Puntgun given: off\n");
	}
	// Check to see if the Puntgun can launch physics
//	ConVar *puntgun_allow_launching = cvar->FindVar( "puntgun_allow_launching" );
//	if (m_bCanLaunch || puntgun_allow_launching->GetBool())
//	{
//		Msg ("Puntgun Launching: on\n");
//	}
//	else
//	{
//		Msg ("Puntgun Launching: off\n");
//	}
	//Check to see if Demo Recording is enabled
	if ( m_bAllowRecording )
	{
		Msg ("Allow Recording: on\n");
	}
	else
	{
		Msg ("Allow Recording: off\n");
	}
	// Check to see if the demo IS recording
	if ( m_bIsRecording )
	{
		Msg ("Is Recording: on\n");
	}
	else
	{
		Msg ("Is Recording: off\n");
	}
	// Check seasons.
	if ( m_intseason == 0 && w_season.GetInt() == 0 )
	{
		Msg ("Season: Spring\n");
	}
//	else
//	{
//		Warning("m_intseason: %i", m_intseason);
//		Warning(". Season Covar does not match the int! Seek a programer!!!\n");
//	}

	if ( m_intseason == 1 && w_season.GetInt() == 1 )
	{
		Msg ("Season: Summer\n");
	}

	if ( m_intseason == 2 && w_season.GetInt() == 2 )
	{
		Msg ("Season: Fall\n");
	}

	if ( m_intseason == 3 && w_season.GetInt() == 3 )
	{
		Msg ("Season: Winter\n");
	}
}

void CpointPuzzleManage::ResetConvars()
{
//	if (!m_bCanLaunch)
//	{
//		ConVar *puntgun_allow_launching = cvar->FindVar( "puntgun_allow_launching" );
//		puntgun_allow_launching->SetValue(0);
//	}

	// Moved to CWorld
	//Gravity
//	ConVar *is_grav_flipped = cvar->FindVar( "is_grav_flipped" );
//	is_grav_flipped->SetValue(0);
		
	// The player is NOT in a cleanser!
//	ConVar *filter_toggle = cvar->FindVar( "filter_toggle" );
//	filter_toggle->SetValue(0);
}
		