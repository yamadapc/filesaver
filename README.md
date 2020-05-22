<p align="center">
  <img src="/design/AppIcon@256px.png" width="200" height="200" />
</p>

<h1 align="center">filesaver</h1>

![CI](https://github.com/yamadapc/filesaver/workflows/CI/badge.svg)

- - -

Very fast multi-threaded file size scanner utility written in C++ and
Objective-C. Scans tens of thousands of files a second. It's able to scan my
whole disk (which 3.5+ million files) in close to a minute.

Doesn't care too much about memory usage. I've implemented a persistence layer,
but since scanning is so fast I've just disabled it on the GUI.

![](/screenshot.png)


![](/diagram.svg)

---

## What is it then?

This is a macOS/CLI application that spawns several worker threads that keep 
scanning your disk to find files' sizes.

A single thread receives results and aggregates them to calculate directory sizes.

The workers work with two queues based on mutexes and condition variables
(**data::WorkQueue**):

* An incoming files **workQueue** queue is polled for entries to scan
* An output **resultsQueue** queue has scanned "FileEntry" objects written to
  (size/type/children)

If the worker thread finds a directory, it'll also enqueue each of the directories'
children onto the **workQueue**, then other workers can pick-up this recursive scan.

On the aggregation side, the consumer/reader thread **FileSaver::entryReader** reads
entries from the resultQueue. It performs 3 operations on every entry:
 
* Set the number of pending children for this entry
* Store the entry onto a cache, where they sit until finished and ready to be stored
* If the entry is a file or is a directory with no pending children
  * Iterate its path up until the root directory
  * For each parent
    * Decrement the number of pending children
    * If the parent is finished, repeat this process for it
  * Free the entry from memory (this will free parent paths that have finished)
* Update the size map for this file and all its parents
  * This is a walk up the tree

- - -

## Performance and background

At this point there aren't very scientific benchmarks for the tool. I can say that I've
profiled it and hand optimized several different parts and that so far it seems the
performance is stellar for its use case of scanning an entire SSD disk and
providing an interactive view of that.

The case where it'll be much slower than simple single-threaded scanners is if the
number of files is small. In this case, the thread sleeping and spawning threads that is
performed will outweight its benefits, or that's how I understood it.

The tool I tried to use before was [`duc`](https://github.com/zevv/duc), which is an awesome
simple C tool for scanning files. It's very fast, but it's single-threaded.

To scan my entire MacBook's disk:

| Size scanned | Files scanned   | Throughput in files (would accept queries) |
|--------------|-----------------|--------------------------------------------|
| 221.5GB      | 5.336.323       | 51389                                      |

## Building

### Requirements

* A modern C++ 17 compiler. (clang / GCC)
* CMake
* Conan package manager
* XCode for the GUI

### Library / CLI

Build with CMake (or the Makefile wrapping it):
```
make build-cli-release
```

### GUI

The XCode project for the GUI is configured for macOS Catalina and newer.

```
make build-gui-release
```

## License

```
The MIT License (MIT)

Copyright (c) 2019 Pedro Tacla Yamada

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

