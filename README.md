The Philosophers (Philo) project at 42 School is a classic multithreading synchronization exercise. Its goal is to teach students how to work with threads, mutexes, and avoid common concurrency problems like race conditions and deadlocks.

🧠 Project Concept: The Dining Philosophers Problem
The original problem is as follows:

Five philosophers sit at a round table.

Each philosopher alternates between thinking, eating, and sleeping.

Between each pair of philosophers is one fork.

To eat, a philosopher needs two forks: the one on their left and the one on their right.

Philosophers must not starve (they must eventually eat).

No two philosophers can use the same fork at the same time (you must protect access to forks).

The simulation should end if a philosopher dies (doesn't eat within time_to_die ms).

🔧 Project Requirements
You must write a program that:

✅ Handles the following arguments:
bash
Copy
Edit
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
number_of_philosophers: how many philosophers/forks

time_to_die: time (ms) before a philosopher dies without eating

time_to_eat: time (ms) a philosopher spends eating

time_to_sleep: time (ms) a philosopher spends sleeping

(optional) number of meals each philosopher must eat before the simulation ends

🔄 Simulation Flow
Each philosopher is a thread, and the forks are protected by mutexes.

A philosopher will:

Take two forks (acquire two mutexes).

Print: has taken a fork

Eat (for time_to_eat ms), then:

Print: is eating

Release forks (unlock mutexes).

Sleep (for time_to_sleep ms)

Print: is sleeping

Think

Print: is thinking

If a philosopher doesn’t eat in time, print:

css
Copy
Edit
[time] [id] died
⚠️ Challenges You Must Handle
Deadlock: If every philosopher grabs one fork and waits for the second, they’ll all block forever.

Race conditions: You must synchronize access to shared data (forks, print, death detection).

Thread-safe printing: Use a print_lock mutex to avoid mixed-up output.

Detecting death: A monitor thread must check if any philosopher has exceeded time_to_die since their last meal.

🧪 Bonus Part (optional)
If you choose to do the bonus, you must:

Use POSIX semaphores instead of mutexes.

Implement the philosophers as processes instead of threads.

🧰 What You'll Learn
Thread creation and management (pthread_create, pthread_join)

Mutex usage (pthread_mutex_*)

Avoiding deadlocks

Handling timing with usleep, gettimeofday

Process creation with fork (bonus)

Semaphore usage (sem_open, sem_wait, sem_post) (bonus)

Writing thread-safe C code
