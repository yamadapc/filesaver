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

- - -

![](/diagram.svg)

---

## Building

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

