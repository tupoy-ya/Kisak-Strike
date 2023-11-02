//========= Copyright 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//
#if !defined( HUD_MACROS_H )
#define HUD_MACROS_H
#ifdef _WIN32
#pragma once
#endif

#include "usermessages.h"

// Macros to hook function calls into the HUD object

#if defined ( CSTRIKE15 )
#define HOOK_MESSAGE(x) \
	m_UMCMsg##x.Bind< CS_UM_##x, CCSUsrMsg_##x >( UtlMakeDelegate( __MsgFunc_##x ) )
#elif defined ( HL2_CLIENT_DLL )
#define HOOK_MESSAGE(x) \
	m_UMCMsg##x.Bind< HL_UM_##x, CHLUsrMsg_##x >( UtlMakeDelegate( __MsgFunc_##x ) )
#endif

#if defined ( CSTRIKE15 )
#define HOOK_HUD_MESSAGE(y, x) \
	m_UMCMsg##x.Bind< CS_UM_##x, CCSUsrMsg_##x >( UtlMakeDelegate( __MsgFunc_##y##_##x ) )
#elif defined ( HL2_CLIENT_DLL )
#define HOOK_HUD_MESSAGE(y, x) \
	m_UMCMsg##x.Bind< HL_UM_##x, CHLUsrMsg_##x >( UtlMakeDelegate( __MsgFunc_##y##_##x ) )
#endif

#if defined ( CSTRIKE15 )
#define HOOK_HUD_MESSAGE_REALTIME_PASSTHROUGH(y, x) \
	m_UMCMsg##x.BindRealtimePassthrough< CS_UM_##x, CCSUsrMsg_##x >( UtlMakeDelegate( __MsgFunc_##y##_##x ) )
#elif defined ( HL2_CLIENT_DLL )
#define HOOK_HUD_MESSAGE_REALTIME_PASSTHROUGH(y, x) \
	m_UMCMsg##x.BindRealtimePassthrough< HL_UM_##x, CHLUsrMsg_##x >( UtlMakeDelegate( __MsgFunc_##y##_##x ) )
#endif


// Message declaration for non-CHudElement classes
#if defined ( CSTRIKE15 )
#define DECLARE_MESSAGE(y, x) bool __MsgFunc_##y##_##x(const CCSUsrMsg_##x &msg) \
	{							\
		return y.MsgFunc_##x( msg );	\
	}
#elif defined ( HL2_CLIENT_DLL )
#define DECLARE_MESSAGE(y, x) bool __MsgFunc_##y##_##x(const CHLUsrMsg_##x &msg) \
	{							\
		return y.MsgFunc_##x( msg );	\
	}
#endif

// Message declaration for CHudElement classes that use the hud element factory for creation
#if defined ( CSTRIKE15 )
#define DECLARE_HUD_MESSAGE(y, x) bool __MsgFunc_##y##_##x(const CCSUsrMsg_##x &msg) \
	{																\
		CHudElement *pElement = GetHud().FindElement( #y );			\
		if ( pElement )												\
		{															\
			return ((y *)pElement)->MsgFunc_##x( msg );				\
		}															\
		return true;												\
	}
#elif defined ( HL2_CLIENT_DLL )
#define DECLARE_HUD_MESSAGE(y, x) bool __MsgFunc_##y##_##x(const CHLUsrMsg_##x &msg) \
	{																\
		CHudElement *pElement = GetHud().FindElement( #y );			\
		if ( pElement )												\
		{															\
			return ((y *)pElement)->MsgFunc_##x( msg );				\
		}															\
		return true;												\
	}
#endif


// Commands
#define HOOK_COMMAND(x, y) static ConCommand x( #x, __CmdFunc_##y, "", FCVAR_SERVER_CAN_EXECUTE );
// Command declaration for non CHudElement classes
#define DECLARE_COMMAND(y, x) void __CmdFunc_##x( void ) \
	{							\
		y.UserCmd_##x( );		\
	}
// Command declaration for CHudElement classes that use the hud element factory for creation
#define DECLARE_HUD_COMMAND(y, x) void __CmdFunc_##x( void )									\
	{																\
		CHudElement *pElement = GetHud().FindElement( #y );				\
		if ( pElement )												\
		{															\
			((y *)pElement)->UserCmd_##x( );						\
		}															\
	}

#define DECLARE_HUD_COMMAND_NAME(y, x, name) void __CmdFunc_##x( void )									\
	{																\
		CHudElement *pElement = GetHud().FindElement( name );			\
		if ( pElement )												\
		{															\
			((y *)pElement)->UserCmd_##x( );						\
		}															\
	}


#endif // HUD_MACROS_H
