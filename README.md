[![Build Version](https://img.shields.io/badge/version-v1.0-yellow.svg)](https://github.com/NeKzor/chaos)
[![Release Status](https://img.shields.io/github/release/NeKzor/chaos/all.svg)](https://github.com/NeKzor/chaos/releases)

## Supported Systems

- Linux
- Windows (TODO)

## Commands & Variables

|Name|Default|Description|
|---|:-:|---|
| chaos_start | cmd | Starts chaos mode. |
| chaos_stop | cmd | Stops chaos mode. |
| chaos_reset_queue | cmd | Resets chaos mode queue. |
| chaos_set_seed | cmd | Sets the seed to generate randomness. |
| chaos_set_quantity | cmd | Sets quantity of specific state. |
| chaos_states | cmd | Prints all possible states. |
| chaos_mode | 0 | Mode of chaos-plugin.<br>0 = Every state occurs multiple times.<br>1 = Every state only occurs once.<br>Every state only occurs once until every state occurred.|
| chaos_time | 30 | How long a state should last, in seconds.<br>-1 = Random number between chaos_time_lower_bound and chaos_time_upper_bound.<br>Value cannot be lower than 10 seconds. |
| chaos_time_lower_bound | 30 | See chaos_time. |
| chaos_time_upper_bound | 40 | See chaos_time. |
| chaos_cooldown | 0 | Amount of seconds to wait until next state occurs.<br>-1 = Random number between chaos_cooldown_lower_bound and chaos_cooldown_upper_bound. |
| chaos_cooldown_lower_bound | 30 | See chaos_cooldown. |
| chaos_cooldown_upper_bound | 40 | See chaos_cooldown. |

## Route Credit

- baister09 (motivation)
- tanger2b (commands, coop testing)
