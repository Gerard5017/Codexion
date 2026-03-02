*This project has been created as part of the 42 curriculum by emarette.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. Multiple coders sit in a circle, each needing two USB dongles simultaneously to compile their quantum code. They cycle through three phases — compile, debug, refactor — and must never burn out (i.e., go too long without compiling).

The simulation ends either when every coder has compiled the required number of times, or when a coder burns out. The goal is to implement correct thread synchronization, deadlock prevention, starvation avoidance, and precise timing — all in C with POSIX threads.

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` binary. Requires `cc` with `-Wall -Wextra -Werror -pthread`.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All arguments are mandatory. All time values are in milliseconds. `scheduler` must be exactly `fifo` or `edf`.

### Examples

```bash
# 5 coders, 800ms to burnout, 200ms compile, 200ms debug, 200ms refactor, 3 compiles required, 50ms cooldown, FIFO scheduler
./codexion 5 800 200 200 200 3 50 fifo

# Same with EDF scheduler
./codexion 5 800 200 200 200 3 50 edf

# Single coder (will burn out immediately — cannot compile with only 1 dongle)
./codexion 1 500 200 200 200 3 50 fifo
```

### Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders and dongles (1–1000) |
| `time_to_burnout` | Max ms between compile starts before burnout |
| `time_to_compile` | Duration of compile phase (holds 2 dongles) |
| `time_to_debug` | Duration of debug phase |
| `time_to_refactor` | Duration of refactor phase |
| `number_of_compiles_required` | Compiles each coder must complete to end simulation |
| `dongle_cooldown` | Ms a dongle is unavailable after being released |
| `scheduler` | `fifo` (arrival order) or `edf` (earliest deadline first) |

## Blocking Cases Handled

### Deadlock Prevention

Deadlock in this problem (analogous to Coffman's conditions) requires: mutual exclusion (dongles held exclusively), hold-and-wait (holding one dongle while waiting for another), no preemption, and circular wait.

We break circular wait by using a **centralized scheduler mutex** (`get_scheduler_mutex()`). All dongle acquisition attempts go through this single global lock, so no circular dependency between dongles can form — a coder either wins the scheduler round or backs off entirely.

### Starvation Prevention

Under `fifo`, requests are served strictly in arrival order via a monotonically incrementing sequence number (`request_seq`), guaranteeing no coder waits forever.

Under `edf`, the coder with the earliest burnout deadline (`last_compile + time_to_burnout`) is prioritized. Ties are broken by `request_seq` (FIFO), ensuring liveness when deadlines are equal.

### Dongle Cooldown

After a coder releases a dongle, it is marked unavailable and a `release_time` is recorded. Any acquisition attempt checks `get_time() - release_time >= dongle_cooldown` before granting access, enforced under the dongle's own mutex.

### Precise Burnout Detection

Burnout is detected in `is_end()`, called by each coder thread at every loop iteration (every 0.5ms during dongle waiting). The exact burnout timestamp is captured **inside** the monitor mutex before the mutex is released, then passed directly to `print_state_ts()`. This ensures the logged timestamp reflects the actual burnout moment, well within the 10ms requirement.

### Log Serialization

All output goes through `print_state_ts()`, which holds a dedicated static `pthread_mutex_t` print mutex around every `printf` call. No two log lines can interleave.

## Thread Synchronization Mechanisms

### `pthread_mutex_t` — Three distinct mutexes

- **`monitor->mutex`**: Protects shared simulation state (`compile_rest`, `end` flag, `last_compile`). Held briefly during state updates in `compile()`, `is_end()`, and `coder_wait()`.
- **`dongle->mutex`** (one per dongle): Protects the `available` flag and `release_time` of each individual dongle. Prevents two coders from simultaneously reading `available = 1` and both taking it.
- **Print mutex** (static in `print_state_ts`): Ensures log lines are atomic.

### Scheduler mutex (`get_scheduler_mutex()`)

A static mutex that wraps the entire dongle-request-and-take sequence in `try_take_dongle()`. It serializes `register_request()` (which assigns sequence numbers and deadlines) and `is_request_winner()` (which scans all pending requests). This prevents TOCTOU races on request priority.

Lock ordering is always: **scheduler_mutex → dongle_mutex**, never the reverse, which eliminates deadlock between these two.

### Race condition prevention examples

**Writing `last_compile`**: done inside `monitor->mutex` in `compile()`. Read in `register_request()` under `scheduler_mutex` (the coder reads its own field, no concurrent writer while under scheduler lock).

**Burnout detection**: `get_time_to_burnout()` reads `last_compile` under `monitor->mutex` in `is_end()` and `coder_wait()`, preventing a torn read while another thread updates it.

**Dongle double-take**: `dongle->available` is set to 0 atomically under `dongle->mutex` inside `try_take_dongle()`, so even if two coders are both declared winners (impossible by design, but defensively), only one can actually take it.

### Monitor thread

A dedicated monitor thread polls the simulation state every 10ms and sets `end = 1` on all coders once `compile_rest <= 0` or a burnout is detected by any coder thread. This provides a safety net for clean termination without relying solely on coder threads to self-terminate.

## Resources

### References

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [The Little Book of Semaphores — Allen B. Downey](https://greenteapress.com/wp/semaphores/)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [pthread_mutex_t man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [gettimeofday man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)

### AI Usage

Claude (Anthropic) was used during this project for:
- **README drafting**: generating this document based on the actual implementation.

All AI-generated suggestions were reviewed, tested, and understood before integration.
