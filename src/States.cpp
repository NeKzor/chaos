#include "States.hpp"

#include "State.hpp"

CHAOS(LSD)
{
    state->on = "r_drawworld 0";
    state->off = "r_drawworld 1";
    state->type = CommandType::ClientSide;
}
CHAOS(Epilepsy)
{
    state->on = "r_farz 128;gl_clear_randomcolor 1";
    state->off = "r_farz -1;gl_clear_randomcolor 0";
    state->type = CommandType::ClientSide;
}
CHAOS(PPNF)
{
    state->on = "sv_portal_placement_never_fail 1";
    state->off = "sv_portal_placement_never_fail 0";
    state->type = CommandType::ServerSide;
}
CHAOS(ThirdPerson)
{
    state->on = "thirdperson";
    state->off = "firstperson";
    state->onceOnly = false;
    state->type = CommandType::ClientSide;
}
CHAOS(PropSpawning)
{
    state->on = "ent_create_portal_weighted_cube;"
        "wait 60;ent_create prop_button;"
        "wait 60;ent_create_portal_weighted_cube";
    state->type = CommandType::ClientSide;
}
CHAOS(EnergyBall)
{
    state->on = "fire_energy_ball";
    state->type = CommandType::ClientSide;
}
CHAOS(TimeScale)
{
    state->on = "host_timescale 2";
    state->off = "host_timescale 1";
    state->type = CommandType::ServerSide;
}
CHAOS(FireDelay)
{
    state->on = "portalgun_fire_delay 3";
    state->off = "portalgun_fire_delay 0.2";
    state->type = CommandType::ServerSide;
}
CHAOS(PortalResize)
{
    state->on = "Portals_ResizeAll 20 50";
    state->off = "Portals_ResizeAll 30 55"; // Dunno the exact size
    state->type = CommandType::ServerSide;
}
CHAOS(Gravity)
{
    state->on = "sv_gravity 300";
    state->off = "sv_gravity 600";
    state->type = CommandType::ServerSide;
}
CHAOS(DualGun)
{
    state->on = "give_portalgun;upgrade_portalgun";
    state->type = CommandType::ClientSide;
}
CHAOS(NoCrosshair)
{
    state->on = "crosshair 0";
    state->off = "crosshair 1";
    state->type = CommandType::ClientSide;
}
CHAOS(NoFriction)
{
    state->on = "sv_friction 0";
    state->off = "sv_friction 4";
    state->type = CommandType::ServerSide;
}
CHAOS(NegativeFriction)
{
    state->on = "sv_friction -4";
    state->off = "sv_friction 4";
    state->type = CommandType::ServerSide;
}
CHAOS(NoPortalFunneling)
{
    state->on = "sv_player_funnel_into_portals 0;sv_props_funnel_into_portals 0";
    state->off = "sv_player_funnel_into_portals 1;sv_props_funnel_into_portals 1";
    state->type = CommandType::ServerSide;
}
CHAOS(PhysTimescale)
{
    state->on = "phys_timescale 0.2";
    state->off = "phys_timescale 1";
    state->type = CommandType::ServerSide;
}
CHAOS(NotSoFastOkay)
{
    state->on = "sv_maxvelocity 250";
    state->off = "sv_maxvelocity 3500";
    state->type = CommandType::ServerSide;
}
CHAOS(RoutingGod)
{
    state->on = "r_drawclipbrushes 2;"
        "vcollide_wireframe 1;"
        "phys_show_active 1;"
        "sv_showhitboxes 1;"
        "developer 2;"
        "contimes 64;"
        "con_notifytime 999;"
        "mat_leafvis 3";
    state->off = "r_drawclipbrushes 0;"
        "vcollide_wireframe 0;"
        "phys_show_active 0;"
        "sv_showhitboxes 0;"
        "developer 0;"
        "contimes 8;"
        "con_notifytime 8;"
        "mat_leafvis 0";
    state->type = CommandType::ServerSide;
}
CHAOS(PassingOut)
{
    state->on = "fadeout %i 0 0 0 255";
    state->off = "fadein 0 0 0 0 0";
    state->isTimed = true;
    state->onceOnly = false;
    state->type = CommandType::ClientSide;
}
CHAOS(AnnoyingUi)
{
    state->on = "demoui;demoui2";
    state->type = CommandType::ClientSide;
}
CHAOS(PortalLinkage)
{
    state->on = "change_portalgun_linkage_id 1";
    state->off = "change_portalgun_linkage_id 0";
    state->onceOnly = false;
    state->type = CommandType::ClientSide;
}
CHAOS(MapSkip)
{
    state->on = "ent_fire @transition_from_map trigger;wait 60;fadein 0 0 0 0 0";
    state->type = CommandType::ServerSide;
}
CHAOS(MansNotHot)
{
    state->on = "ent_fire !self ignite;ent_fire !self ignitelifetime %i";
    state->off = "ent_fire !self ignitelifetime 0";
    state->isTimed = true;
    state->onceOnly = false;
    state->type = CommandType::ClientSide;
}
