# ssTs: Small &amp; Simple Task Scheduler for C++17

**ssTs** is a time-based *Task Scheduler*, written in modern C++.  
Header only, with [no external dependencies](#tests).

![ssTs](/logo/ssTs_logo.png)

## Integration

### Header only
Copy the [include](/include) folder, that contains the 3 header files [task.hpp](/include/ssts/task.hpp), [task_pool.hpp](/include/ssts/task_pool.hpp) and [task_scheduler.hpp](/include/ssts/task_scheduler.hpp) within your project or set your include path to it and just build your code.  
**ssTs** requires a *C++17* compiler.

### CMake
It is possible to *install* the library using:
```console
$ git clone https://github.com/stefanolusardi/task_scheduler.git
$ cd ssts && mkdir build && cd build
$ cmake -G<GENERATOR> -DCMAKE_BUILD_TYPE=<Debug|Release> -DSSTS_INSTALL_LIBRARY=ON -DCMAKE_INSTALL_PREFIX=</install/folder/path> ..
$ cmake --build . --config <Debug|Release> --target install 
```

## Platforms & Compilers

### Developed and tested on:
*   Windows 10 - Visual Studio 2019
*   Windows 10 - Clang 10.0
*   Windows 10 - Clang 9.0
*   Ubuntu 20.04 - GCC 9.3
*   Ubuntu 20.04 - Clang 9.0
*   Ubuntu 20.04 - Clang 10.0

### Supported CI platforms:

#### Linux:

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu2004_GCC9)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 20.04 - GCC 9.3

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu1804_GCC9)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 18.04 - GCC 9.3

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu1804_GCC7)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 18.04 - GCC 7.5

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu1804_GCC8)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 18.04 - GCC 8.4

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu2004_GCC7)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 20.04 - GCC 7.5

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu2004_GCC8)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 20.04 - GCC 8.4

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu1804_Clang8)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 18.04 - Clang 8.0

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu1804_Clang9)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 18.04 - Clang 9.0

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu2004_Clang8)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 20.04 - Clang 8.0

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu2004_Clang9)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 20.04 - Clang 9.0

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Ubuntu2004_Clang10)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Ubuntu 20.04 - Clang 10.0


#### Windows:

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Windows2019_VisualStudio2019)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Windows Server 2019 - Visual Studio 2019

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Windows2016_VisualStudio2017)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Windows Server 2016 - Visual Studio 2017

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Windows2019_Clang10)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Windows Server 2019 - Clang 10.0

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=Windows2016_Clang10)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
Windows Server 2016 - Clang 10.0


#### MacOS:

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=MacOSMojave1014_Clang10)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
MacOS X Mojave 10.14 - Clang 10.0

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=MacOSMojave1014_GCC8)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
MacOS X Mojave 10.14 - GCC 8.4

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=MacOSMojave1014_GCC9)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
MacOS X Mojave 10.14 - GCC 9.3

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=MacOSCatalina1015_Clang10)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
MacOS X Catalina 10.15 - Clang 10.0

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=MacOSCatalina1015_GCC8)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
MacOS X Catalina 10.15 - GCC 8.4

* [![Build Status](https://stefanolusardi.visualstudio.com/ssts/_apis/build/status/ssts_pipeline?branchName=master&jobName=MacOSCatalina1015_GCC9)](https://stefanolusardi.visualstudio.com/ssts/_build/latest?definitionId=14&branchName=master)
MacOS X Catalina 10.15 - GCC 9.3


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
It is possible to build and install the *examples* using:
```console
$ git clone https://github.com/stefanolusardi/task_scheduler.git
$ cd ssts && mkdir build && cd build
$ cmake -G<GENERATOR>  -DCMAKE_BUILD_TYPE=<Debug|Release> -DSSTS_BUILD_EXAMPLES=ON  -DSSTS_INSTALL_LIBRARY=ON -DSSTS_INSTALL_EXAMPLES=ON ..
$ cmake --build . --config <Debug|Release>
```

## Tests
*Tests* are located within the [tests](/tests) folder.  
[GoogleTest](https://github.com/google/googletest) is the only 3rd party library used and it is required only to build the tests target.
It is integrated using [DownloadGoogleTest.cmake](/cmake/DownloadGoogleTest.cmake) script. 
It is possible to build the *tests* using:
```console
$ git clone https://github.com/stefanolusardi/task_scheduler.git
$ cd ssts && mkdir build && cd build
$ cmake -G<GENERATOR>  -DCMAKE_BUILD_TYPE=<Debug|Release> -DSSTS_BUILD_TESTS=ON ..
$ cmake --build . --config <Debug|Release>
```
