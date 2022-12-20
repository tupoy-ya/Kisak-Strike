#//-----------------------------------------------------------------------------
#//	CLIENT_CSTRIKE15.VPC
#//
#//	Project Script
#//-----------------------------------------------------------------------------

#There are other projects in this folder, but this cmake is focused towards csgo a.k.a. cstrike15

cmake_minimum_required(VERSION 3.16)
project(Client_Portal)
set(SRCDIR "${CMAKE_SOURCE_DIR}")
set(GAMENAME "portal")
set(PORTAL "1")
#set(DONT_DOWNGRADE_ABI "1")

#dont need to build this when building dedicated server
if( DEDICATED )
    return()
endif()

add_definitions(-DHL2_CLIENT_DLL -DPORTAL -DGAMEUI_EMBEDDED -DGAMEUI_EXPORTS)
if( USE_SCALEFORM )
    add_definitions(-DINCLUDE_SCALEFORM)
elseif( USE_ROCKETUI )
    add_definitions(-DINCLUDE_ROCKETUI)
endif()
add_definitions(-DALLOW_TEXT_MODE=1)

include( ./client_base.cmake )

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/portal)
include_directories(${SRCDIR}/game/shared/multiplayer)
include_directories(${SRCDIR}/game/shared/portal)
include_directories(${SRCDIR}/game/shared/hl2)
include_directories(${SRCDIR}/game/shared)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hl2)

#cstrike15 protos
TargetBuildAndAddProto( ${OUTBINNAME} ${SRCDIR}/game/shared/cstrike15/cstrike15_gcmessages.proto "${GENERATED_PROTO_DIR}")
TargetBuildAndAddProto( ${OUTBINNAME} ${SRCDIR}/game/shared/cstrike15/cstrike15_usermessages.proto "${GENERATED_PROTO_DIR}")
TargetBuildAndAddProto( ${OUTBINNAME} ${SRCDIR}/gcsdk/gcsdk_gcmessages.proto "${GENERATED_PROTO_DIR}")
TargetBuildAndAddProto( ${OUTBINNAME} ${SRCDIR}/gcsdk/gcsystemmsgs.proto "${GENERATED_PROTO_DIR}")

#force include
if( WINDOWS OR POSIX )
    target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/public/tier0/platform.h")
endif()

#instead we add csgo checks to client_base.cmake
#-$File	"${SRCDIR}/game/shared/weapon_parse_default.cpp"
#-$File	"c_mod_lesson_stubs.cpp"

#32bit time?

#$Folder	"Source Files"
target_sources(${OUTBINNAME} PRIVATE "hud_chat.cpp")
target_sources(${OUTBINNAME} PRIVATE "c_team_objectiveresource.cpp")

#$Folder	"HL2 DLL"
target_sources(${OUTBINNAME} PRIVATE "episodic/flesh_internal_material_proxy.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/hl2/basehlcombatweapon_shared.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_antlion_dust.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_ar2_explosion.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_barnacle.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_barney.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_basehelicopter.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_basehlcombatweapon.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_basehlplayer.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_citadel_effects.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_corpse.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_env_alyxtemp.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_env_headcrabcanister.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_env_starfield.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_func_tankmortar.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_hl2_playerlocaldata.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_info_teleporter_countdown.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_npc_antlionguard.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_npc_combinegunship.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_npc_manhack.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_npc_rollermine.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_plasma_beam_node.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_prop_combine_ball.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_rotorwash.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_script_intro.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/script_intro_shared.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_strider.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_te_concussiveexplosion.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_te_flare.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_thumper_dust.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_vehicle_airboat.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_vehicle_cannon.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_vehicle_crane.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_vehicle_prisoner_pod.cpp")
target_sources(${OUTBINNAME} PRIVATE "episodic/c_vort_charge_token.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_weapon_crossbow.cpp")
target_sources(${OUTBINNAME} PRIVATE "episodic/c_weapon_hopwire.cpp")
target_sources(${OUTBINNAME} PRIVATE "episodic/c_vehicle_jeep_episodic.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_radar.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/c_weapon_stunstick.cpp")
target_sources(${OUTBINNAME} PRIVATE "death.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/hl2/env_headcrabcanister_shared.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/fx_antlion.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/fx_bugbait.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/fx_hl2_impacts.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/fx_hl2_tracers.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/hl2/hl2_gamerules.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/hl2/hl_gamemovement.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hl_in_main.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hl_prediction.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_ammo.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_battery.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_blood.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_bonusprogress.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_credits.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_damageindicator.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_flashlight.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_health.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_poisondamageindicator.cpp")
target_sources(${OUTBINNAME} PRIVATE "hud_squadstatus.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_suitpower.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_weaponselection.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/hud_zoom.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/shieldproxy.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/hl2/survival_gamerules.cpp")
target_sources(${OUTBINNAME} PRIVATE "hl2/vgui_rootpanel_hl2.cpp")

#$Folder	"Portal"
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/achievements_portal.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_env_lightraill_endpoint.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_env_portal_path_track.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_func_liquidportal.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_neurotoxin_countdown.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_npc_portal_turret_floor.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_npc_rocket_turret.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_portal_player.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/C_PortalGhostRenderable.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_prop_energy_ball.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_prop_portal.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_prop_portal_stats_display.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/clientmode_portal.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/fx_portal.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/hud_quickinfo.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/MaterialProxy_Portal_PickAlphaMask.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/materialproxy_portalstatic.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/multiplayer/multiplayer_animstate.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_collideable_enumerator.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/portal_credits.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/Portal_DynamicMeshRenderingUtils.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_gamemovement.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_gamerules.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/portal_hud_crosshair.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_player_shared.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_playeranimstate.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/portal_render_targets.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_shareddefs.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_usermessages.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_util_shared.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/prop_portal_shared.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/PortalSimulation.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/StaticCollisionPolyhedronCache.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/PortalRender.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_portal_radio.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/portalrenderable_flatbasic.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/vgui_portal_stats_display_screen.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/vgui_neurotoxin_countdown_screen.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/portal_weapon_parse.cpp")

#$Folder	"Weapons"
target_sources(${OUTBINNAME} PRIVATE "hl2/c_weapon_gravitygun.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_weapon_physcannon.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_weapon_portalgun.cpp")
target_sources(${OUTBINNAME} PRIVATE "portal/c_weapon_portalgun.h")
target_sources(${OUTBINNAME} PRIVATE "portal/c_weapon_stubs_portal.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/weapon_portalbase.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/weapon_portalbase.h")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/weapon_portalbasecombatweapon.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/weapon_portalbasecombatweapon.h")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/weapon_portalgun_shared.cpp")
target_sources(${OUTBINNAME} PRIVATE "${SRCDIR}/game/shared/portal/weapon_portalgun_shared.h")



#$Folder "hltvreplay"
#{
    #target_sources(${OUTBINNAME} PRIVATE "hltvreplaysystem.cpp")
#}

target_link_libraries(${OUTBINNAME} meshutils_client)
if( WINDOWS OR POSIX )
    target_link_libraries(${OUTBINNAME} jpeglib_client)
    target_link_libraries(${OUTBINNAME} vpklib_client)
endif()
target_link_libraries(${OUTBINNAME} matsys_controls_client)
target_link_libraries(${OUTBINNAME} ${LIBPUBLIC}/libsteam_api.so) #proprietary steam api
target_link_libraries(${OUTBINNAME} kisak_gcsdk_client)
target_link_libraries(${OUTBINNAME} tier2_client tier3_client vgui_controls_client vtf_client bzip2_client libvstdlib_client dmxloader_client bitmap_client mathlib_client tier2_client )
#target_link_libraries(${OUTBINNAME} protobuf )

