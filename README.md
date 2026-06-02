*This project has been created as part of the 42 curriculum by wagyu.*

# Description

Codexion is a POSIX threads concurrency simulation based on the Dining
Philosophers problem. Each coder alternates between compiling, debugging, and
refactoring. Compiling requires exclusive access to two adjacent USB dongles.

Each dongle has a cooldown period after release. Requests are ordered with a
binary heap according to the selected scheduler:

- `fifo`: first request arrival first.
- `edf`: earliest burnout deadline first.

The simulation stops when every coder completes the required number of
compiles, or when a coder burns out.

# Instructions

Compile the project:

```sh
make
```

Run the simulation:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
	time_to_debug time_to_refactor number_of_compiles_required \
	dongle_cooldown scheduler
```

Example:

```sh
./codexion 5 1000 200 100 100 3 20 edf
```

All durations are in milliseconds. The scheduler must be `fifo` or `edf`.

Remove generated files:

```sh
make fclean
```

# Resources

- POSIX threads documentation:
  https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html
- `pthread_cond_timedwait` documentation:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html
- `gettimeofday` documentation:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/gettimeofday.html
- Binary heap overview:
  https://en.wikipedia.org/wiki/Binary_heap
- Dining Philosophers problem:
  https://en.wikipedia.org/wiki/Dining_philosophers_problem

AI was used to help design the project structure, review synchronization edge
cases, generate an initial implementation, and prepare verification commands.
The resulting C code, scheduling behavior, cleanup paths, README, and test
results must still be understood and defended by the student.

# Blocking cases handled

- Circular wait is avoided by atomically claiming both required dongles.
- Dongle mutexes are always acquired in index order to avoid lock-order
  deadlocks.
- Each dongle queue prevents duplicate ownership and applies deterministic
  arbitration.
- Released dongles remain unavailable until their cooldown timestamp passes.
- A separate monitor thread detects burnout and stops all waiting threads.
- The single-coder case takes the only available dongle and burns out without
  attempting to lock the same mutex twice.
- Initialization failures and normal shutdown free allocated resources.

# Thread synchronization mechanisms

- `pthread_mutex_t` protects the simulation state.
- One `pthread_mutex_t` per dongle protects its owner, cooldown timestamp, and
  request heap.
- A dedicated `pthread_mutex_t` serializes output lines.
- `pthread_cond_t` wakes coders when ownership, cooldown, deadlines, or the stop
  state changes.
- `pthread_cond_timedwait` waits until the nearest relevant timestamp without
  busy waiting.
