# ssTs: Small &amp; Simple Task Scheduler for C++17

Header only time-based task scheduler written in modern C++

![ssTs](/logo/ssTs_logo.png)

## Integration

### Header only
Copy the [include]() folder, that contains the 3 header files [task.hpp](), [task_pool.hpp]() and [task_scheduler.hpp]() within your project or set your include path to it and just build your code.  
ssTs Requires a C++17 compiler.

### CMake
soon

## Platforms & Compilers
Tested and developed on:
*   Windows 10 - MSVC 16
*   Ubuntu 20.04 - GCC 9

## Basic Usage
```cpp
// Allow literal suffixes (e.g. 3s, 250ms)
using namespace std::chrono_literals;

// Create a Task Scheduler instance
ts::task_scheduler s(2);

// Spawn a one-shot task in 5 seconds
s.in(5s, []{std::cout << "Hello!" << std::endl;});
```

```cpp
// Launch a task with parameters and wait for its return value
std::future f = s.at(std::chrono::steady_clock::now() + 2s, 
[](auto x, auto y){ 
    std::cout << "Input parameters: " << x << y << std::endl;
    return x+1; 
}, 42, "fourty-two");

std::cout << "Task result: " << f.get() << std::endl; // prints 42
```

```cpp
// It's possible to start a task giving a string as task id to be able to manipulate it later.

// Check if a task is currently scheduled 
// (i.e. already inserted in the scheduler)
s.is_scheduled("my_task_id"); // false

// Check if a task is currently enabled
// (i.e. inserted and allowed to execute, by default any task is enabled)
s.is_enabled("my_task_id"); // false

// Start a recursive task with a task id that is scheduled every second
s.every("my_task_id", 1s, [](auto p){std::cout << "Hello! " << p << std::endl;}, "some_task_parameter");
s.is_scheduled("my_task_id"); // true
s.is_enabled("my_task_id"); // true

// Disable "my_task_id"
s.set_enabled("my_task_id", false);
s.is_scheduled("my_task_id"); // true
s.is_enabled("my_task_id"); // false

// Remove "my_task_id"
s.remove_task("my_task_id"s);
s.is_scheduled("my_task_id"); // false
s.is_enabled("my_task_id"); // false

// Stop Task Scheduler (by default it's always properly stopped when it goes out of scope)
s.stop();
```

## Examples
Examples are located within the [examples](/examples) folder.  
It is possible to build all the examples using:
```console
$ git clone https://github.com/stefanolusardi/ssts.git
$ cd ssts && mkdir build && cd build
$ cmake --build . --config Release -DBUILD_EXAMPLES=ON
```

## Tests
Coming soon
