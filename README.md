# ssTs: Small &amp; Simple Task Scheduler for C++17

Time-based Task Scheduler, written in modern C++
Header only, with [no external dependencies](#tests).

![ssTs](/logo/ssTs_logo.png)

## Integration

### Header only
Copy the [include](/include) folder, that contains the 3 header files [task.hpp](/include/ssts/task.hpp), [task_pool.hpp](/include/ssts/task_pool.hpp) and [task_scheduler.hpp](/include/ssts/task_scheduler.hpp) within your project or set your include path to it and just build your code.  
**ssTs** Requires a *C++17* compiler.

### CMake
It is possible to *install* the library using:
```console
$ git clone https://github.com/stefanolusardi/task_scheduler.git
$ cd ssts && mkdir build && cd build
$ cmake -DSSTS_INSTALL=ON -DCMAKE_INSTALL_PREFIX=</install/folder/path> ..
$ cmake --build . --config Release --target install 
```

## Platforms & Compilers
Tested and developed on:
*   Windows 10 - Visual Studio 2019
*   Ubuntu 20.04 - GCC 9.3

Supported CI platforms (coming soon):
*   Windows Server 2016 - Visual Studio 2017
*   Windows Server 2019 - Visual Studio 2019
*   Windows Server 2019 - Clang 10.0

*   Ubuntu 18.04 - GCC 7.5
*   Ubuntu 18.04 - GCC 9.3
*   Ubuntu 18.04 - GCC 8.4

*   Ubuntu 18.04 - Clang 8.0
*   Ubuntu 18.04 - Clang 9.0
*   Ubuntu 18.04 - Clang 10.0

*   Ubuntu 20.04 - GCC 7.5
*   Ubuntu 20.04 - GCC 8.4
*   Ubuntu 20.04 - GCC 9.3

*   Ubuntu 20.04 - Clang 8.0
*   Ubuntu 20.04 - Clang 9.0
*   Ubuntu 20.04 - Clang 10.0

## Basic Usage
* Running a task is as simple as:
```cpp
// Allow literal suffixes (e.g. 3s, 250ms)
using namespace std::chrono_literals;

// Create a Task Scheduler instance
ts::task_scheduler s(2);

// Spawn a one-shot task in 5 seconds
s.in(5s, []{std::cout << "Hello!" << std::endl;});
```

* To retrieve a task result you can do:
```cpp
// Launch a task with parameters and wait for its return value
std::future f = s.at(std::chrono::steady_clock::now() + 2s, 
[](auto x, auto y){ 
    std::cout << "Input parameters: " << x << y << std::endl;
    return x+1; 
}, 42, "fourty-two");

std::cout << "Task result: " << f.get() << std::endl; // prints 43
```

* It's possible to start a task giving it a task id to be able to manipulate it later.
```cpp
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
*Examples* are located within the [examples](/examples) folder.  
It is possible to build all the *examples* using:
```console
$ git clone https://github.com/stefanolusardi/task_scheduler.git
$ cd ssts && mkdir build && cd build
$ cmake --build . --config Release -DSSTS_BUILD_EXAMPLES=ON
```

## Tests
*Tests* are located within the [tests](/tests) folder.  
*Googletest* is the only 3rd party library used and it is required only to build the tests target.
It is integrated using [DownloadGoogleTest.cmake](/cmake/DownloadGoogleTest.cmake) script. 
It is possible to build the *tests* using:
```console
$ git clone https://github.com/stefanolusardi/task_scheduler.git
$ cd ssts && mkdir build && cd build
$ cmake --build . --config Release -DSSTS_BUILD_TESTS=ON
```
