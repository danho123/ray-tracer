## Ray Tracer

Ray Tracer currently builds on Linux.

### Prerequisites

On Debian-based Linuxes:

<<<<<<< HEAD
    sudo apt-get libglew1.6-dev freeglut3-dev
=======
    sudo apt-get install libsdl1.2-dev libcairo2-dev libpango1.0-dev autoconf2.13 freeglut3-dev
>>>>>>> e11411530577a26f0ab9a2030515c173450ac1cc

### Building

    git clone https://github.com/danho123/ray-tracer
<<<<<<< HEAD
    cd ray-tracer/raycast
    make +d 5 +s +p +l
    ./raycast
=======
    cd servo
    git submodule init
    git submodule update
    ./autogen.sh
    mkdir -p build && cd build
    ../configure
    make check && make
    ./servo ../src/test/test.html
>>>>>>> e11411530577a26f0ab9a2030515c173450ac1cc
