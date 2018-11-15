#include "Callbacks.hpp"

#include "Modules/Engine.hpp"

#include "Command.hpp"
#include "RandomCallback.hpp"
#include "Variable.hpp"

CHAOS(LSD)
{
    static Variable r_drawworld;
    static Variable gl_clear_randomcolor;

    if (!state->initialized) {
        r_drawworld = Variable("r_drawworld");
        gl_clear_randomcolor = Variable("gl_clear_randomcolor");
        state->initialized = !!r_drawworld && !!gl_clear_randomcolor;
        return;
    }

    if (lucky) {
        r_drawworld.SetValue(0);
        gl_clear_randomcolor.SetValue(1);
    } else {
        r_drawworld.SetValue(1);
        gl_clear_randomcolor.SetValue(0);
    }
}

CHAOS(PPNF)
{
    static Variable sv_portal_placement_never_fail;

    if (!state->initialized) {
        sv_portal_placement_never_fail = Variable("sv_portal_placement_never_fail");
        state->initialized = !!sv_portal_placement_never_fail;
        return;
    }

    if (lucky) {
        sv_portal_placement_never_fail.SetValue(1);
    } else {
        sv_portal_placement_never_fail.SetValue(0);
    }
}

CHAOS(ThirdPerson)
{
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("thirdperson");
    } else {
        engine->SendToCommandBuffer("firstperson");
    }
}

CHAOS(PropSpawning)
{
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("ent_create portal_weighted_cube");
        engine->SendToCommandBuffer("npc_create npc_portal_turret_floor", 60);
        engine->SendToCommandBuffer("ent_create prop_button", 120);
    }
}

CHAOS(EnergyBall)
{
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("fire_energy_ball");
    }
}

CHAOS(TimeScale)
{
    static Variable host_timescale;

    if (!state->initialized) {
        host_timescale = Variable("host_timescale");
        state->initialized = !!host_timescale;
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

    if (!state->initialized) {
        fps_max = Variable("fps_max");
        fps_max_splitscreen = Variable("fps_max_splitscreen");

        if (state->initialized = !!fps_max && !!fps_max_splitscreen) {
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

    if (!state->initialized) {
        portalgun_fire_delay = Variable("portalgun_fire_delay");
        state->initialized = !!portalgun_fire_delay;
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
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("Portals_ResizeAll 20 50");
    }
}

CHAOS(Gravity)
{
    static Variable sv_gravity;

    if (!state->initialized) {
        sv_gravity = Variable("sv_gravity");

        if (state->initialized = !!sv_gravity) {
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

CHAOS(FakeLag)
{
    static Variable net_fakelag;

    if (!state->initialized) {
        net_fakelag = Variable("net_fakelag");
        state->initialized = !!net_fakelag;
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
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("give_portalgun");
        engine->SendToCommandBuffer("upgrade_portalgun");
    }
}

CHAOS(NoCrosshair)
{
    static Variable r_drawvgui;
    if (!state->initialized) {
        r_drawvgui = Variable("r_drawvgui");
        state->initialized = !!r_drawvgui;
        return;
    }

    if (lucky) {
        r_drawvgui.SetValue(0);
    } else {
        r_drawvgui.SetValue(1);
    }
}

CHAOS(NoFriction)
{
    static Variable sv_friction;

    if (!state->initialized) {
        sv_friction = Variable("sv_friction");

        if (state->initialized = !!sv_friction) {
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

CHAOS(NoPortalFunneling)
{
    static Variable sv_player_funnel_into_portals;

    if (!state->initialized) {
        sv_player_funnel_into_portals = Variable("sv_player_funnel_into_portals");
        state->initialized = !!sv_player_funnel_into_portals;
        return;
    }

    if (lucky) {
        sv_player_funnel_into_portals.SetValue(0);
    } else {
        sv_player_funnel_into_portals.SetValue(1);
    }
}

/* CHAOS(MapSkip)
{
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("transition_map");
    }
} */

/* CHAOS(FOV)
{
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        //cl_fov 200
    } else {
        //cl_fov 90
    }
} */

/* CHAOS(MapRestart)
{
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("restart_level");
    }
} */

/* CHAOS(RipSettings)
{
    if (!state->initialized) {
        state->initialized = true;
        return;
    }

    if (lucky) {
        engine->SendToCommandBuffer("unbindall");
        engine->SendToCommandBuffer("sensitivity 13");
    }
} */
