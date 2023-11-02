//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include "gameinterface.h"
#include "mapentities.h"
#include "fmtstr.h"
#include "server_log_http_dispatcher.h"
#include "usermessages.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

void CServerGameClients::GetPlayerLimits( int& minplayers, int& maxplayers, int &defaultMaxPlayers ) const
{
#ifdef PORTAL2
	minplayers = defaultMaxPlayers = 1;
	maxplayers = CommandLine()->FindParm( "-allowspectators" ) ? 3 : 2;
#else
	minplayers = defaultMaxPlayers = 1; 
	maxplayers = MAX_PLAYERS;
#endif
}


// -------------------------------------------------------------------------------------------- //
// Mod-specific CServerGameDLL implementation.
// -------------------------------------------------------------------------------------------- //

void CServerGameDLL::LevelInit_ParseAllEntities( const char *pMapEntities )
{
}

void CServerGameDLL::ApplyGameSettings( KeyValues *pKV )
{
	if ( !pKV )
	{
		return;
	}

	if ( engine )
	{
		DevMsg( "CServerGameDLL::ApplyGameSettings game settings payload received:\n" );
		KeyValuesDumpAsDevMsg( pKV, 1 );

		const char* pMapName = NULL;
		const char* pMapNameFromKV = pKV->GetString( "game/map" );
		const char* pGameType = pKV->GetString( "game/type" );
		const char* pGameMode = pKV->GetString( "game/mode" );
		const char* pMapGroupName = pKV->GetString( "game/mapgroupname", NULL );
		const char* pMapGroupNameToValidate = NULL;		// pMapGroupName is ok to be NULL; this variable lets us easily use a non null pMapGroupName or gpGlobals->mapGroupName

		if ( !IsValveDS() &&
			pMapNameFromKV && StringHasPrefix( pMapNameFromKV, "workshop" ) &&
			pMapGroupName && Q_stristr( pMapGroupName, "@workshop" ) )
		{
			// A community server is getting reserved by a client for a workshop map,
			// retain our current workshop collection if we are hosting one to preserve
			// map rotation process
			pMapGroupName = engine->IsDedicatedServer() ? STRING( gpGlobals->mapGroupName ) : pMapGroupName;
		}
		{
			pMapGroupNameToValidate = ( pMapGroupName && (pMapGroupName[0] != '\0') ) ? pMapGroupName : STRING( gpGlobals->mapGroupName );
		}

		// make sure we are not using a bogus mapgroup name
		if ( pMapGroupNameToValidate && !StringIsEmpty( pMapGroupNameToValidate ) )
		{
			Warning( "ApplyGameSettings: Invalid mapgroup name %s\n", pMapGroupNameToValidate );
			return;
		}

		// only use the map name from the pKV if there was no mapgroup name in the pKV
		if ( !pMapName )
		{
			pMapName = pMapNameFromKV;
		}

		// For team games we add the prefix "team" to the game type. This is to
		// eliminate team game lobbies from searches for QuickMatch and Custom Match
		const char *teamStr = "team";
		const char *pTeamPrefix = Q_strstr( pGameType, teamStr);
		if ( pTeamPrefix == pGameType )
		{
			pGameType += Q_strlen( teamStr );
		}

		if ( pMapName && pMapName[0] != '\0' )
		{
			int extraSpectators = 2;

			if ( ( pGameType && pGameType[0] != '\0' ) &&
				 ( pGameMode && pGameMode[0] != '\0' ) )
			{

				// Get the bot difficulty setting before it gets reverted.
				ConVarRef cvCustomBotDiff( "custom_bot_difficulty" );
				int customBotDiff = cvCustomBotDiff.GetInt();

/*
				// FIXME[pmf]: We don't want to reset all replicated convars unless we also re-exec game.cfg on the server,
				// otherwise we'll overwrite all the game configuration convars specified in game.cfg

				// Reset server enforced convars
				g_pCVar->RevertFlaggedConVars( FCVAR_REPLICATED );
*/

				// Cheats were disabled; revert all cheat cvars to their default values.
				// This must be done heading into multiplayer games because people can play
				// demos etc and set cheat cvars with sv_cheats 0.
				g_pCVar->RevertFlaggedConVars( FCVAR_CHEAT );

				// we know that the loading screen data is correct, so let the loading screen know
				//g_pGameTypes->SetLoadingScreenDataIsCorrect( true );
				//g_pGameTypes->SetRunMapWithDefaultGametype( false );
				// Set game_type and game_mode convars.
				//g_pGameTypes->SetGameTypeAndMode( pGameType, pGameMode );

#if defined ( CSTRIKE15 )
				extern ConVar game_online;
				if ( char const *szOnline = pKV->GetString( "system/network", NULL ) )
				{
					game_online.SetValue( ( !V_stricmp( szOnline, "LIVE" ) ) ? 1 : 0 );
				}
				
				extern ConVar game_public;
				if ( char const *szAccess = pKV->GetString( "system/access", NULL ) )
				{
					game_public.SetValue( ( !V_stricmp( szAccess, "public" ) ) ? 1 : 0 );
				}

#ifndef CLIENT_DLL
				if ( engine->IsDedicatedServer() )
					game_online.SetValue( 1 );
#endif

#endif

				// Make sure the settings keys have extra spectator info
				pKV->SetInt( "members/numExtraSpectatorSlots", extraSpectators );
			}

			CFmtStr command;

			if ( pMapGroupName )
			{
				command.AppendFormat( "mapgroup %s\n", pMapGroupName );
			}
			command.AppendFormat( "nextlevel %s\n", pMapName ); // gamerules will clean it up when they construct for the next map
			command.AppendFormat( "map %s reserved\n", pMapName );
			
			Warning( "Executing server command:\n%s\n---\n", command.Access() );
			engine->ServerCommand( command );
			if ( engine->IsDedicatedServer() )
				engine->ServerExecute();
		}
	}
}

void CServerGameDLL::UpdateGCInformation()
{
	/** Removed for partner depot **/
}

// Marks the queue matchmaking game as starting
void CServerGameDLL::ReportGCQueuedMatchStart( int32 iReservationStage, uint32 *puiConfirmedAccounts, int numConfirmedAccounts )
{
	/** Removed for partner depot **/
}

//-----------------------------------------------------------------------------
// Purpose: A user has had their network id setup and validated 
//-----------------------------------------------------------------------------
void CServerGameClients::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID, CSteamID steamID )
{
	/** Removed for partner depot **/
}

//
// Matchmaking game data buffer to set into SteamGameServer()->SetGameData
//
void CServerGameDLL::GetMatchmakingGameData( char *buf, size_t bufSize )
{
	char * const bufBase = buf;
	int len = 0;

	// Put the game key
	Q_snprintf( buf, bufSize, "g:csgo," );
	len = strlen( buf );
	buf += len;
	bufSize -= len;

	// Trim the last comma if anything was written
	if ( buf > bufBase )
		buf[ -1 ] = 0;
}

// validate if player is a caster and is not playing in the current game, then add them to the active caster list
// returns false if they are not allow to be a caster
bool CServerGameDLL::ValidateAndAddActiveCaster( const CSteamID &steamID )
{
	return false;
}

// Returns which encryption key to use for messages to be encrypted for TV
EncryptedMessageKeyType_t CServerGameDLL::GetMessageEncryptionKey( INetMessage *pMessage )
{
	return kEncryptedMessageKeyType_None;
}

// If server game dll needs more time before server process quits then
// it should return true to hold game server reservation from this interface method.
// If this method returns false then the server process will clear the reservation
// and might shutdown to meet uptime or memory limit requirements.
bool CServerGameDLL::ShouldHoldGameServerReservation( float flTimeElapsedWithoutClients )
{
	/** Removed for partner depot **/
	return false; // let the server get unreserved
}

// Pure server validation failed for the given client, client supplied
// data is included in the payload
void CServerGameDLL::OnPureServerFileValidationFailure( edict_t *edictClient, const char *path, const char *fileName, uint32 crc, int32 hashType, int32 len, int packNumber, int packFileID )
{
	/** Removed for partner depot **/
}

// Last chance validation on connect packet for the client, non-NULL return value
// causes the client connect to be aborted with the provided error
char const * CServerGameDLL::ClientConnectionValidatePreNetChan( bool bGameServer, char const *adr, int nAuthProtocol, uint64 ullSteamID )
{
	/** Removed for partner depot **/
	return NULL;	// allow connections by default
}

// Network channel notification from engine to game server code
void CServerGameDLL::OnEngineClientNetworkEvent( edict_t *edictClient, uint64 ullSteamID, int nEventType, void *pvParam )
{
	/** Removed for partner depot **/
}

// Game server notifying GC with its sync packet
void CServerGameDLL::EngineGotvSyncPacket( const CEngineGotvSyncPacket *pPkt )
{
	/** Removed for partner depot **/
}

// GOTV client attempt redirect over SDR
bool CServerGameDLL::OnEngineClientProxiedRedirect( uint64 ullClient, const char *adrProxiedRedirect, const char *adrRegular )
{
	/** Removed for partner depot **/
	return false;
}

bool CServerGameDLL::LogForHTTPListeners( const char* szLogLine )
{
	return GetServerLogHTTPDispatcher()->LogForHTTPListeners( szLogLine );
}




const char * CServerGameClients::ClientNameHandler( uint64 xuid, const char *pchName )
{
	// Account not resolved, use whatever name they provided
	return pchName;
}

void CServerGameClients::ClientSvcUserMessage( edict_t *pEntity, int nType, int nPassthrough, uint32 cbSize, const void *pvBuffer )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetContainingEntity( pEntity ) );
	if ( !pPlayer )
		return;

	/*
	switch ( nType )
	{
	case HL_UM_PlayerDecalDigitalSignature:
		{
			CHLUsrMsg_PlayerDecalDigitalSignature msg;
			if ( msg.ParseFromArray( pvBuffer, cbSize ) )
				pPlayer->SprayPaint( msg );
		}
		return;
	}
	*/
}
