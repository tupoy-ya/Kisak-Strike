//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ====
//
// Purpose: 
//
//=============================================================================
#include "cbase.h"
#include "clientmode_hlnormal.h"
#include "vgui_int.h"
#include "hud.h"
#include <vgui/IInput.h>
#include <vgui/IPanel.h>
#include <vgui/ISurface.h>
#include <vgui_controls/AnimationController.h>
#include "iinput.h"
#include "vgui_int.h"
#include "ienginevgui.h"
#include "cdll_client_int.h"
#include "engine/IEngineSound.h"

#include "ivmodemanager.h"
#include "panelmetaclassmgr.h"
//#include "nb_header_footer.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern bool g_bRollingCredits;
ConVar default_fov( "default_fov", "75", FCVAR_CHEAT );
ConVar fov_desired( "fov_desired", "75", FCVAR_ARCHIVE | FCVAR_USERINFO, "Sets the base field-of-view.", true, 75.0, true, 90.0 );

vgui::HScheme g_hVGuiCombineScheme = 0;

static IClientMode *g_pClientMode[ MAX_SPLITSCREEN_PLAYERS ];
IClientMode *GetClientMode()
{
	ASSERT_LOCAL_PLAYER_RESOLVABLE();
	return g_pClientMode[ GET_ACTIVE_SPLITSCREEN_SLOT() ];
}

// --------------------------------------------------------------------------------- //
// CASWModeManager.
// --------------------------------------------------------------------------------- //

class CSDKModeManager : public IVModeManager
{
public:
	virtual void	Init();
	virtual void	SwitchMode( bool commander, bool force ) {}
	virtual void	LevelInit( const char *newmap );
	virtual void	LevelShutdown( void );
	virtual void	ActivateMouse( bool isactive ) {}
};


static CSDKModeManager g_ModeManager;
IVModeManager *modemanager = ( IVModeManager * )&g_ModeManager;
// --------------------------------------------------------------------------------- //
// CASWModeManager implementation.
// --------------------------------------------------------------------------------- //

#define SCREEN_FILE		"scripts/vgui_screens.txt"

void CSDKModeManager::Init()
{
	for( int i = 0; i < MAX_SPLITSCREEN_PLAYERS; ++i )
	{
		ACTIVE_SPLITSCREEN_PLAYER_GUARD( i );
		g_pClientMode[ i ] = GetClientModeNormal();
	}

	PanelMetaClassMgr()->LoadMetaClassDefinitionFile( SCREEN_FILE );
}

void CSDKModeManager::LevelInit( const char *newmap )
{
	for( int i = 0; i < MAX_SPLITSCREEN_PLAYERS; ++i )
	{
		ACTIVE_SPLITSCREEN_PLAYER_GUARD( i );
		GetClientMode()->LevelInit( newmap );
	}
}

void CSDKModeManager::LevelShutdown( void )
{
	for( int i = 0; i < MAX_SPLITSCREEN_PLAYERS; ++i )
	{
		ACTIVE_SPLITSCREEN_PLAYER_GUARD( i );
		GetClientMode()->LevelShutdown();
	}
}

ClientModeHLNormal g_ClientModeNormal[ MAX_SPLITSCREEN_PLAYERS ];
IClientMode *GetClientModeNormal()
{
	Assert( engine->IsLocalPlayerResolvable() );
	return &g_ClientModeNormal[ engine->GetActiveSplitScreenPlayerSlot() ];
}

ClientModeHLNormal* GetClientModeSDK()
{
	Assert( engine->IsLocalPlayerResolvable() );
	return &g_ClientModeNormal[ engine->GetActiveSplitScreenPlayerSlot() ];
}


static char const *s_CloseWindowNames[]={
	"InfoMessageWindow",
	"SkipIntro",
};

//-----------------------------------------------------------------------------
// Purpose: this is the viewport that contains all the hud elements
//-----------------------------------------------------------------------------
class CHudViewport : public CBaseViewport
{
private:
	DECLARE_CLASS_SIMPLE( CHudViewport, CBaseViewport );

protected:
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme )
	{
		BaseClass::ApplySchemeSettings( pScheme );

		GetHud().InitColors( pScheme );

		SetPaintBackgroundEnabled( false );
	}

	virtual void CreateDefaultPanels( void ) { /* don't create any panels yet*/ };
};


bool ClientModeHLNormal::ShouldDrawCrosshair( void )
{
	return ( g_bRollingCredits == false );
}


//--------------------------------------------------------------------------------------------------------

// See interface.h/.cpp for specifics:  basically this ensures that we actually Sys_UnloadModule the dll and that we don't call Sys_LoadModule 
//  over and over again.
static CDllDemandLoader g_GameUI( "gameui" );

class FullscreenSDKViewport : public CHudViewport
{
private:
	DECLARE_CLASS_SIMPLE( FullscreenSDKViewport, CHudViewport );

private:
	virtual void InitViewportSingletons( void )
	{
		SetAsFullscreenViewportInterface();
	}
};

class ClientModeHLNormalFullscreen : public	ClientModeHLNormal
{
	DECLARE_CLASS_SIMPLE( ClientModeHLNormalFullscreen, ClientModeHLNormal );
public:
	virtual void InitViewport()
	{
		// Skip over BaseClass!!!
		BaseClass::BaseClass::InitViewport();
		m_pViewport = new FullscreenSDKViewport();
		m_pViewport->Start( gameuifuncs, gameeventmanager );
	}
	virtual void Init()
	{
		// 
		//CASW_VGUI_Debug_Panel *pDebugPanel = new CASW_VGUI_Debug_Panel( GetViewport(), "ASW Debug Panel" );
		//g_hDebugPanel = pDebugPanel;

		// Skip over BaseClass!!!
		BaseClass::BaseClass::Init();

		// Load up the combine control panel scheme
		if ( !g_hVGuiCombineScheme )
		{
			g_hVGuiCombineScheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), IsX360() ? "resource/ClientScheme.res" : "resource/CombinePanelScheme.res", "CombineScheme" );
			if (!g_hVGuiCombineScheme)
			{
				Warning( "Couldn't load combine panel scheme!\n" );
			}
		}
	}
	void Shutdown()
	{
	}
};

//--------------------------------------------------------------------------------------------------------
static ClientModeHLNormalFullscreen g_FullscreenClientMode;
IClientMode *GetFullscreenClientMode( void )
{
	return &g_FullscreenClientMode;
}


void ClientModeHLNormal::Init()
{
	BaseClass::Init();

	gameeventmanager->AddListener( this, "game_newmap", false );
	// Load up the combine control panel scheme
	g_hVGuiCombineScheme = vgui::scheme()->LoadSchemeFromFileEx( enginevgui->GetPanel( PANEL_CLIENTDLL ), IsX360() ? "resource/ClientScheme.res" : "resource/CombinePanelScheme.res", "CombineScheme" );
	if (!g_hVGuiCombineScheme)
	{
		Warning( "Couldn't load combine panel scheme!\n" );
	}
}
void ClientModeHLNormal::Shutdown()
{
}

void ClientModeHLNormal::InitViewport()
{
	m_pViewport = new CHudViewport();
	m_pViewport->Start( gameuifuncs, gameeventmanager );
}

void ClientModeHLNormal::LevelInit( const char *newmap )
{
	// reset ambient light
	static ConVarRef mat_ambient_light_r( "mat_ambient_light_r" );
	static ConVarRef mat_ambient_light_g( "mat_ambient_light_g" );
	static ConVarRef mat_ambient_light_b( "mat_ambient_light_b" );

	if ( mat_ambient_light_r.IsValid() )
	{
		mat_ambient_light_r.SetValue( "0" );
	}

	if ( mat_ambient_light_g.IsValid() )
	{
		mat_ambient_light_g.SetValue( "0" );
	}

	if ( mat_ambient_light_b.IsValid() )
	{
		mat_ambient_light_b.SetValue( "0" );
	}

	m_hCurrentColorCorrection = NULL;

	BaseClass::LevelInit(newmap);

	// sdk: make sure no windows are left open from before
	// SDK_CloseAllWindows();

	// clear any DSP effects
	CLocalPlayerFilter filter;
	enginesound->SetRoomType( filter, 0 );
	enginesound->SetPlayerDSP( filter, 0, true );
}

void ClientModeHLNormal::LevelShutdown( void )
{
	BaseClass::LevelShutdown();

	// sdk:shutdown all vgui windows
	// SDK_CloseAllWindows();
}
void ClientModeHLNormal::FireGameEvent( IGameEvent *event )
{
	const char *eventname = event->GetName();

	if ( Q_strcmp( "asw_mission_restart", eventname ) == 0 )
	{
		// SDK_CloseAllWindows();
	}
	else if ( Q_strcmp( "game_newmap", eventname ) == 0 )
	{
		engine->ClientCmd("exec newmapsettings\n");
	}
	else
	{
		BaseClass::FireGameEvent(event);
	}
}

void ClientModeHLNormal::DoPostScreenSpaceEffects( const CViewSetup *pSetup )
{

}

void ClientModeHLNormal::OnColorCorrectionWeightsReset( void )
{
//	BaseClass::OnColorCorrectionWeightsReset();

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	
	/*
	// if the player is dead, fade in the death color correction
	if ( m_CCDeathHandle != INVALID_CLIENT_CCHANDLE && ( pPlayer != NULL ) )
	{
		if ( pPlayer->m_lifeState != LIFE_ALIVE )
		{
			if ( m_flDeathCCWeight < 1.0f )
			{
				m_flDeathCCWeight += DEATH_CC_FADE_SPEED;
				clamp( m_flDeathCCWeight, 0.0f, 1.0f );
			}

			// Player is dead, fade out the environmental color correction
			if ( m_hCurrentColorCorrection )
			{
				m_hCurrentColorCorrection->EnableOnClient( false );
				m_hCurrentColorCorrection = NULL;
			}
		}
		else 
		{
			m_flDeathCCWeight = 0.0f;
		}

		g_pColorCorrectionMgr->SetColorCorrectionWeight( m_CCDeathHandle, m_flDeathCCWeight );
	}

	// Only blend between environmental color corrections if there is no death-induced color correction
	if ( m_flDeathCCWeight == 0.0f )
	*/
	{
		C_ColorCorrection *pNewColorCorrection = pPlayer ? pPlayer->GetActiveColorCorrection() : NULL;
		C_ColorCorrection *pOldColorCorrection = m_hCurrentColorCorrection;

		if ( pNewColorCorrection != pOldColorCorrection )
		{
			if ( pOldColorCorrection )
			{
				pOldColorCorrection->EnableOnClient( false );
			}
			if ( pNewColorCorrection )
			{
				pNewColorCorrection->EnableOnClient( true, pOldColorCorrection == NULL );
			}
			m_hCurrentColorCorrection = pNewColorCorrection;
		}
	}
}
