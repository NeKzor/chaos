#include "Cheats.hpp"

#include <cstring>

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"

#include "Chaos.hpp"

Variable chaos_mode = Variable("chaos_mode", "0",
    "Mode of chaos-plugin. "
    "0 = Default, "
    "1 = Dispatch only once, "
    "2 = Dispatch only once and then reset.\n");
Variable chaos_delay = Variable("chaos_delay", "30",
    "Amount of seconds to wait until next dispatch. "
    "0 = Random number between chaos_delay_lower_bound and chaos_delay_upper_bound.\n");
Variable chaos_delay_lower_bound = Variable("chaos_delay_lower_bound", "30",
    "See chaos_delay.\n");
Variable chaos_delay_upper_bound = Variable("chaos_delay_upper_bound", "40",
    "See chaos_delay.\n");

void Cheats::Init()
{
    Variable::RegisterAll();
    Command::RegisterAll();
}
void Cheats::Shutdown()
{
    Variable::UnregisterAll();
    Command::UnregisterAll();
}

CON_COMMAND(chaos_start, "Start chaos mode.\n")
{
    if (!sv_cheats.GetBool())
        sv_cheats.SetValue(1);

    chaos.hasStarted = true;
    console->Print("Started chaos!\n");
}
CON_COMMAND(chaos_stop, "Stop chaos mode.\n")
{
    chaos.hasStarted = false;
    console->Print("Stopped chaos!\n");
}
CON_COMMAND(chaos_reset, "Reset chaos mode queue.\n")
{
    chaos.ResetCallbacks();
    console->Print("Reset chaos!\n");
}
