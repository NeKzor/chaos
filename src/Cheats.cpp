#include "Cheats.hpp"

#include <cstring>

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"

#include "Chaos.hpp"

Variable chaos_mode = Variable("chaos_mode", "0",
    "Mode of chaos-plugin. "
    "0 = Every state occurs multiple times, "
    "1 = Every state only occurs once, "
    "2 = Every state only occurs once until every state occurred.\n");
Variable chaos_time = Variable("chaos_time", "30",
    "How long a state should last in seconds. "
    "-1 = Random number between \"chaos_time_lower_bound\" and \"chaos_time_upper_bound\". "
    "Note: Value cannot be lower than 10 seconds.\n");
Variable chaos_time_lower_bound = Variable("chaos_time_lower_bound", "30",
    "See chaos_time.\n");
Variable chaos_time_upper_bound = Variable("chaos_time_upper_bound", "40",
    "See chaos_time.\n");
Variable chaos_cooldown = Variable("chaos_cooldown", "0",
    "Amount of seconds to wait until the next state occurs. "
    "-1 = Random number between \"chaos_cooldown_lower_bound\" and \"chaos_cooldown_upper_bound\".\n");
Variable chaos_cooldown_lower_bound = Variable("chaos_cooldown_lower_bound", "30",
    "See chaos_cooldown.\n");
Variable chaos_cooldown_upper_bound = Variable("chaos_cooldown_upper_bound", "40",
    "See chaos_cooldown.\n");

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

CON_COMMAND(chaos_start, "Starts chaos mode.\n")
{
    if (chaos.isRunning) {
        return console->Print("Chaos mode already running!\n");
    }

    // Secretly activate cheats which won't notify the player but is required
    if (!sv_cheats.GetBool()) {
        sv_cheats.SetValue(1);
    }

    chaos.Start();
    console->Print("Started chaos!\n");
}
CON_COMMAND(chaos_stop, "Stops chaos mode.\n")
{
    if (!chaos.isRunning) {
        return console->Print("Chaos mode not running!\n");
    }

    chaos.Stop();
    console->Print("Stopped chaos!\n");
}
CON_COMMAND(chaos_reset_queue, "Resets chaos mode queue.\n")
{
    chaos.Reset();
    console->Print("Reset queue!\n");
}
CON_COMMAND(chaos_set_seed, "Sets the seed to generate randomness.\n")
{
    if (args.ArgC() != 2) {
        return console->Print("Current seed: %u\n", chaos.seed);
    }

    if (chaos.isRunning) {
        return console->Print("Cannot set seed during chaos mode!\n");
    }

    auto seed = std::atoi(args[1]);
    if (seed < 0) {
        return console->Print("Seed cannot be negative!\n");
    }

    chaos.SetSeed(seed);
    console->Print("New seed: %u\n", chaos.seed);
}
CON_COMMAND(chaos_set_quantity, "Sets quantity of specific state. "
                                "Usage: chaos_set_quantity <state_name> <quantity_number>\n")
{
    if (args.ArgC() != 3) {
        return console->Print("Sets quantity of specific state. "
                              "Usage: chaos_set_quantity <state_name> <quantity_number>\n");
    }

    if (chaos.isRunning) {
        return console->Print("Cannot set quantity during chaos mode!\n");
    }

    for (const auto& state : State::list) {
        if (!std::strcmp(args[1], state->name)) {
            auto quantity = std::atoi(args[2]);
            console->Print("Changed quantity of %s from %i to %i.\n", state->name, state->quantity, quantity);
            state->quantity = quantity;
            chaos.Reset();
            break;
        }
    }
}
CON_COMMAND(chaos_states, "Prints all possible states.\n")
{
    auto total = chaos.queue.size();
    console->Msg("state-name (probability)\n");
    for (const auto& state : State::list) {
        auto probability = ((float)state->quantity / total) * 100;
        if (!state->initialized) {
            console->Warning("%s (%.2f)\n", state->name, probability);
        } else if (chaos.curState && !std::strcmp(chaos.curState->name, state->name)) {
            console->PrintActive("%s (%.2f)\n", state->name, probability);
        } else {
            console->Print("%s (%.2f)\n", state->name, probability);
        }
    }

    if (chaos.isRunning) {
        console->Print("Probabilities are not accurate during chaos mode.\n");
    }
}
