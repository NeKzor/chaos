#include "Callbacks.hpp"

#include <algorithm>

#include "Chaos.hpp"
#include "Cheats.hpp"
#include "Command.hpp"
#include "State.hpp"
#include "Variable.hpp"

CHAOS(LSD)
{
    static Variable r_drawworld;

    if (!state->isInitialized) {
        r_drawworld = Variable("r_drawworld");
        state->isInitialized = !!r_drawworld;
        return;
    }

    if (lucky) {
        r_drawworld.SetValue(0);
    } else {
        r_drawworld.SetValue(1);
    }
}

CHAOS(Epilepsy)
{
    static Variable r_farz;
    static Variable gl_clear_randomcolor;

    if (!state->isInitialized) {
        r_farz = Variable("r_farz");
        gl_clear_randomcolor = Variable("gl_clear_randomcolor");
        state->isInitialized = !!r_farz && !!gl_clear_randomcolor;
        return;
    }

    if (lucky) {
        r_farz.SetValue(128);
        gl_clear_randomcolor.SetValue(1);
    } else {
        r_farz.SetValue(-1);
        gl_clear_randomcolor.SetValue(0);
    }
}

CHAOS(PPNF)
{
    static Variable sv_portal_placement_never_fail;

    if (!state->isInitialized) {
        sv_portal_placement_never_fail = Variable("sv_portal_placement_never_fail");
        state->isInitialized = !!sv_portal_placement_never_fail;
        return;
    }

    if (lucky) {
        sv_portal_placement_never_fail.SetValue(1);
    } else {
        sv_portal_placement_never_fail.SetValue(0);
    }
}

CHAOS2(ThirdPerson)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("thirdperson");
    } else {
        chaos.EachClient("firstperson");
    }
}

CHAOS(PropSpawning)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("ent_create_portal_weighted_cube");
        chaos.EachClient("ent_create prop_button");
        chaos.EachClient("ent_create_portal_weighted_cube");
    }
}

CHAOS(EnergyBall)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("fire_energy_ball");
    }
}

CHAOS(TimeScale)
{
    static Variable host_timescale;

    if (!state->isInitialized) {
        host_timescale = Variable("host_timescale");
        state->isInitialized = !!host_timescale;
        return;
    }

    if (lucky) {
        host_timescale.SetValue(2);
    } else {
        host_timescale.SetValue(1);
    }
}

CHAOS(FPS)
{
    static Variable fps_max;
    static Variable fps_max_splitscreen;

    static int fps_max_default;
    static int fps_max_splitscreen_default;

    if (!state->isInitialized) {
        fps_max = Variable("fps_max");
        fps_max_splitscreen = Variable("fps_max_splitscreen");

        if (state->isInitialized = !!fps_max && !!fps_max_splitscreen) {
            fps_max_default = fps_max.GetInt();
            fps_max_splitscreen_default = fps_max_splitscreen.GetInt();
        }
        return;
    }

    if (lucky) {
        fps_max.SetValue(15);
        fps_max_splitscreen.SetValue(15);
    } else {
        fps_max.SetValue(fps_max_default);
        fps_max_splitscreen.SetValue(fps_max_splitscreen_default);
    }
}

CHAOS(FireDelay)
{
    static Variable portalgun_fire_delay;

    if (!state->isInitialized) {
        portalgun_fire_delay = Variable("portalgun_fire_delay");
        state->isInitialized = !!portalgun_fire_delay;
        return;
    }

    if (lucky) {
        portalgun_fire_delay.SetValue(3.00f);
    } else {
        portalgun_fire_delay.SetValue(0.20f);
    }
}

CHAOS(PortalResize)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.BufferCommand("Portals_ResizeAll 20 50");
    } else {
        chaos.BufferCommand("Portals_ResizeAll 30 55"); // Dunno the exact size
    }
}

CHAOS(Gravity)
{
    static Variable sv_gravity;

    if (!state->isInitialized) {
        sv_gravity = Variable("sv_gravity");

        if (state->isInitialized = !!sv_gravity) {
            sv_gravity.Notify(false);
        }
        return;
    }

    if (lucky) {
        sv_gravity.SetValue(300);
    } else {
        sv_gravity.SetValue(600);
    }
}

CHAOS3(FakeLag)
{
    static Variable net_fakelag;

    if (!state->isInitialized) {
        net_fakelag = Variable("net_fakelag");
        state->isInitialized = !!net_fakelag;
        return;
    }

    if (lucky) {
        net_fakelag.SetValue(100);
    } else {
        net_fakelag.SetValue(0);
    }
}

CHAOS(DualGun)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("give_portalgun");
        chaos.EachClient("upgrade_portalgun");
    }
}

CHAOS(NoCrosshair)
{
    static Variable crosshair;

    if (!state->isInitialized) {
        crosshair = Variable("crosshair");
        state->isInitialized = !!crosshair;
        return;
    }

    if (lucky) {
        crosshair.SetValue(0);
    } else {
        crosshair.SetValue(1);
    }
}

CHAOS(NoFriction)
{
    static Variable sv_friction;

    if (!state->isInitialized) {
        sv_friction = Variable("sv_friction");

        if (state->isInitialized = !!sv_friction) {
            sv_friction.Notify(false);
        }
        return;
    }

    if (lucky) {
        sv_friction.SetValue(0);
    } else {
        sv_friction.SetValue(4);
    }
}

CHAOS(NegativeFriction)
{
    static Variable sv_friction;

    if (!state->isInitialized) {
        sv_friction = Variable("sv_friction");

        if (state->isInitialized = !!sv_friction) {
            sv_friction.Notify(false);
        }
        return;
    }

    if (lucky) {
        sv_friction.SetValue(-4);
    } else {
        sv_friction.SetValue(4);
    }
}

CHAOS(NoPortalFunneling)
{
    static Variable sv_player_funnel_into_portals;
    static Variable sv_props_funnel_into_portals;

    if (!state->isInitialized) {
        sv_player_funnel_into_portals = Variable("sv_player_funnel_into_portals");
        sv_props_funnel_into_portals = Variable("sv_props_funnel_into_portals");
        state->isInitialized = !!sv_player_funnel_into_portals && !!sv_props_funnel_into_portals;
        return;
    }

    if (lucky) {
        sv_player_funnel_into_portals.SetValue(0);
        sv_props_funnel_into_portals.SetValue(0);
    } else {
        sv_player_funnel_into_portals.SetValue(1);
        sv_props_funnel_into_portals.SetValue(1);
    }
}

CHAOS(PhysTimescale)
{
    static Variable phys_timescale;

    if (!state->isInitialized) {
        phys_timescale = Variable("phys_timescale");
        state->isInitialized = !!phys_timescale;
        return;
    }

    if (lucky) {
        phys_timescale.SetValue(0.2f);
    } else {
        phys_timescale.SetValue(1);
    }
}

CHAOS(NotSoFastOkay)
{
    static Variable sv_maxvelocity;

    if (!state->isInitialized) {
        sv_maxvelocity = Variable("sv_maxvelocity");
        state->isInitialized = !!sv_maxvelocity;
        return;
    }

    if (lucky) {
        sv_maxvelocity.SetValue(250);
    } else {
        sv_maxvelocity.SetValue(3500);
    }
}

CHAOS(RoutingGod)
{
    static Variable r_drawclipbrushes;
    static Variable vcollide_wireframe;
    static Variable phys_show_active;
    static Variable sv_debug_player_use;
    static Variable sv_showhitboxes;
    static Variable developer;
    static Variable contimes;
    static Variable con_notifytime;
    static Variable mat_leafvis;

    if (!state->isInitialized) {
        r_drawclipbrushes = Variable("r_drawclipbrushes");
        vcollide_wireframe = Variable("vcollide_wireframe");
        phys_show_active = Variable("phys_show_active");
        sv_debug_player_use = Variable("sv_debug_player_use");
        sv_showhitboxes = Variable("sv_showhitboxes");
        developer = Variable("developer");
        contimes = Variable("contimes");
        con_notifytime = Variable("con_notifytime");
        mat_leafvis = Variable("mat_leafvis");
        state->isInitialized = !!r_drawclipbrushes
            && !!vcollide_wireframe
            && !!phys_show_active
            && !!sv_debug_player_use
            && !!sv_showhitboxes
            && !!developer
            && !!contimes
            && !!con_notifytime
            && !!mat_leafvis;
        return;
    }

    if (lucky) {
        r_drawclipbrushes.SetValue(2);
        vcollide_wireframe.SetValue(1);
        phys_show_active.SetValue(1);
        sv_debug_player_use.SetValue(1);
        sv_showhitboxes.SetValue(1);
        developer.SetValue(2);
        contimes.SetValue(64);
        con_notifytime.SetValue(999);
        mat_leafvis.SetValue(3);
        //chaos.EachClient("developer 2");
    } else {
        r_drawclipbrushes.SetValue(0);
        vcollide_wireframe.SetValue(0);
        phys_show_active.SetValue(0);
        sv_debug_player_use.SetValue(0);
        sv_showhitboxes.SetValue(0);
        developer.SetValue(0);
        contimes.SetValue(8);
        con_notifytime.SetValue(8);
        mat_leafvis.SetValue(0);
        //chaos.EachClient("developer 0");
    }
}

CHAOS2(PassingOut)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("fadeout %i 0 0 0 255", std::max(chaos_time.GetInt(), 10));
    } else {
        chaos.EachClient("fadein 0 0 0 0 0");
    }
}

CHAOS(AnnoyingUi)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("demoui");
        chaos.EachClient("demoui2");
        //chaos.EachClient("fogui");
        //chaos.EachClient("open_econui");
    }
}

CHAOS2(PortalLinkage)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("change_portalgun_linkage_id 1");
    } else {
        chaos.EachClient("change_portalgun_linkage_id 0");
    }
}

CHAOS(MapSkip)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.BufferCommand("ent_fire @transition_from_map trigger");
        chaos.BufferCommand("fadein 0 0 0 0 0", 60);
    }
}

CHAOS2(MansNotHot)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.EachClient("ent_fire !self ignite");
        chaos.EachClient("ent_fire !self ignitelifetime %i", std::max(chaos_time.GetInt(), 10));
    } else {
        chaos.EachClient("ent_fire !self ignitelifetime 0");
    }
}

CHAOS2(MotionBlur)
{
    static Variable mat_motion_blur_enabled;
    static Variable mat_motion_blur_falling_min;
    static Variable mat_motion_blur_percent_of_screen_max;
    static Variable mat_motion_blur_strength;

    static int default_mat_motion_blur_enabled;
    static float default_mat_motion_blur_falling_min;
    static float default_mat_motion_blur_percent_of_screen_max;
    static float default_mat_motion_blur_strength;

    if (!state->isInitialized) {
        mat_motion_blur_enabled = Variable("mat_motion_blur_enabled");
        mat_motion_blur_falling_min = Variable("mat_motion_blur_falling_min");
        mat_motion_blur_percent_of_screen_max = Variable("mat_motion_blur_percent_of_screen_max");
        mat_motion_blur_strength = Variable("mat_motion_blur_strength");
        if (state->isInitialized = !!mat_motion_blur_enabled
                && !!mat_motion_blur_falling_min
                && !!mat_motion_blur_percent_of_screen_max
                && !!mat_motion_blur_strength) {
            default_mat_motion_blur_enabled = mat_motion_blur_enabled.GetInt();
            default_mat_motion_blur_falling_min = mat_motion_blur_falling_min.GetFloat();
            default_mat_motion_blur_percent_of_screen_max = mat_motion_blur_percent_of_screen_max.GetFloat();
            default_mat_motion_blur_strength = mat_motion_blur_strength.GetFloat();
        }
    }

    if (lucky) {
        mat_motion_blur_enabled.SetValue(1);
        mat_motion_blur_falling_min.SetValue(999);
        mat_motion_blur_percent_of_screen_max.SetValue(100.0f);
        mat_motion_blur_strength.SetValue(999);
    } else {
        mat_motion_blur_enabled.SetValue(default_mat_motion_blur_enabled);
        mat_motion_blur_falling_min.SetValue(default_mat_motion_blur_falling_min);
        mat_motion_blur_percent_of_screen_max.SetValue(default_mat_motion_blur_percent_of_screen_max);
        mat_motion_blur_strength.SetValue(default_mat_motion_blur_strength);
    }
}

/* CHAOS(RipSettings)
{
    if (!state->isInitialized) {
        state->isInitialized = true;
        return;
    }

    if (lucky) {
        chaos.BufferCommand("unbindall");
        chaos.BufferCommand("sensitivity 13");
    }
} */
