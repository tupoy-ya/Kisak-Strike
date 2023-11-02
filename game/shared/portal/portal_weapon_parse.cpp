//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//

#include "cbase.h"
#include <KeyValues.h>
#include "portal_weapon_parse.h"
#include "ammodef.h"

FileWeaponInfo_t* CreateWeaponInfo()
{
#ifdef PORTAL2
	return new FileWeaponInfo_t;
#else
	return new CPortalSWeaponInfo;
#endif
}



CPortalSWeaponInfo::CPortalSWeaponInfo()
{
	m_flPlayerDamage = 0;
}


void CPortalSWeaponInfo::Parse( KeyValues *pKeyValuesData, const char *szWeaponName )
{
	BaseClass::Parse( pKeyValuesData, szWeaponName );

	m_flPlayerDamage = pKeyValuesData->GetInt( "damage", 0 );
}


