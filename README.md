[![Build Version](https://img.shields.io/badge/version-v1.0-yellow.svg)](https://github.com/NeKzor/chaos)
[![Release Status](https://img.shields.io/github/release/NeKzor/chaos/all.svg)](https://github.com/NeKzor/chaos/releases)

## Supported Systems

- Linux
- Windows (TODO)

## Commands & Variables

|ConCommand|Default|Description|
|---|:-:|---|
| chaos_start | cmd | Start chaos mode. |
| chaos_stop | cmd | Stop chaos mode. |
| chaos_reset | cmd | Reset chaos mode queue. |
| chaos_mode | 0 | Mode of chaos-plugin.<br>0 = Default.<br>1 = Dispatch only once.<br>2 = Dispatch only once and then reset.|
| chaos_delay | 30 | Amount of seconds to wait until next dispatch.<br>0 = Random number between chaos_delay_lower_bound and chaos_delay_upper_bound. |
| chaos_delay_lower_bound | 30 | See chaos_mode. |
| chaos_delay_upper_bound | 40 | See chaos_mode. |
