# Philosophers

## Description

The program "philo" simulates philosophers using **threads and mutexes** to synchronize their actions. The specific rules for the mandatory part are as follows:

- Each philosopher should be implemented as a **separate thread**.
- There is one fork between each pair of philosophers. Therefore, if there are several philosophers, each philosopher has a fork on their left side and a fork on their right side. If there is only one philosopher, there should be only one fork on the table.
- To prevent philosophers from duplicating forks, you should protect the forks' state with a **mutex** for each of them.

## Turn-in Files

Please include the following files in the `philo/` or `philo_bonus/` directory:

- Makefile
- All *.h header files
- All *.c source files

## Makefile

The Makefile should contain the following targets:

- `NAME`: The name of the executable file to be generated.
- `all`: Builds the executable.
- `clean`: Removes object files.
- `fclean`: Removes object files and the executable.
- `re`: Performs a clean build by removing object files and rebuilding the executable.

## Arguments

The program `philo` should accept the following command-line arguments:

```
number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers`: The number of philosophers and also the number of forks.
- `time_to_die` (in milliseconds): If a philosopher doesn't start eating within `time_to_die` milliseconds since their last meal or the start of the simulation, they die.
- `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat.
- `time_to_sleep` (in milliseconds): The time a philosopher spends sleeping.
- `number_of_times_each_philosopher_must_eat` (optional): If all philosophers have eaten at least `number_of_times_each_philosopher_must_eat` times, the simulation stops. If not specified, the simulation stops when a philosopher dies.

## External Functions

You are authorized to use the following external functions:

- [`memset`](https://www.man7.org/linux/man-pages/man3/memset.3.html)
- [`printf`](https://www.man7.org/linux/man-pages/man3/printf.3.html)
- [`malloc`](https://www.man7.org/linux/man-pages/man3/malloc.3.html)
- [`free`](https://www.man7.org/linux/man-pages/man3/free.3.html)
- [`write`](https://www.man7.org/linux/man-pages/man2/write.2.html)
- [`usleep`](https://www.man7.org/linux/man-pages/man3/usleep.3.html)
- [`gettimeofday`](https://www.man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [`pthread_create`](https://www.man7.org/linux/man-pages/man3/pthread_create.3.html)
- [`pthread_detach`](https://www.man7.org/linux/man-pages/man3/pthread_detach.3.html)
- [`pthread_join`](https://www.man7.org/linux/man-pages/man3/pthread_join.3.html)
- [`pthread_mutex_init`](https://www.man7.org/linux/man-pages/man3/pthread_mutex_init.3.html)
- [`pthread_mutex_destroy`](https://www.man7.org/linux/man-pages/man3/pthread_mutex_destroy.3.html)
- [`pthread_mutex_lock`](https://www.man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html)
- [`pthread_mutex_unlock`](https://www.man7.org/linux/man-pages/man3/pthread_mutex_unlock.3.html)

## Libft

You are not authorized to use the `libft` library for this assignment.

## Overview

This assignment involves simulating a scenario where philosophers sit at a round table and alternate between eating, thinking, and sleeping.\
The simulation has the following rules:

- There is a large bowl of spaghetti in the middle of the table.
- Philosophers can either eat, think, or sleep, but not simultaneously.
- While eating, philosophers are not thinking or sleeping.
- While thinking, philosophers are not eating or sleeping.
- While sleeping, philosophers are not eating or thinking.
- There are as many forks on the table as there are philosophers.
- Each philosopher takes both the right and left forks to eat.
- After finishing eating, the philosopher puts the forks back on the table and starts sleeping.
- Once awake, the philosopher starts thinking again.
- The simulation stops when a philosopher dies of starvation.
- Every philosopher needs to eat and should never starve.
- Philosophers do not communicate with each other.
- Philosophers are unaware if another philosopher is about to die.

## Global rules

To complete the assignment, you need to write two programs: one for the mandatory part and another for the bonus part (optional). Both programs must adhere to the following rules:

- Global variables are not allowed.
- Your program(s) should accept the following arguments:
- Each philosopher is assigned a number from 1 to `number_of_philosophers`.
- Philosopher number 1 sits next to philosopher number `number_of_philosophers`. Any other philosopher number N sits between philosopher number N - 1 and philosopher number N + 1.

## Logs

- Any state change of a philosopher must be formatted as follows:
  - `timestamp_in_ms X has taken a fork`
  - `timestamp_in_ms X is eating`
  - `timestamp_in_ms X is sleeping`
  - `timestamp_in_ms X is thinking`
  - `timestamp_in_ms X died`

  Replace `timestamp_in_ms` with the current timestamp in milliseconds, and X with the philosopher number.

- Each displayed state message should not be mixed up with another message.
- A message announcing a philosopher's death should be displayed no more than 10 ms after the philosopher's actual death.
- Once again, philosophers should avoid dying!

Your program must be designed to avoid any [data races](https://en.wikipedia.org/wiki/Race_condition#Data_race).
