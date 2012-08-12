## Ray Tracer

Ray Tracer currently builds on Linux.

### Prerequisites

On Debian-based Linuxes:

    sudo apt-get install libsdl1.2-dev libcairo2-dev libpango1.0-dev autoconf2.13 freeglut3-dev

### Building

    git clone https://github.com/danho123/ray-tracer
    cd servo
    git submodule init
    git submodule update
    ./autogen.sh
    mkdir -p build && cd build
    ../configure
    make check && make
    ./servo ../src/test/test.html
