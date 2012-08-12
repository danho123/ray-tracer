## Ray Tracer

Ray Tracer currently builds on Linux.

### Prerequisites

On Debian-based Linuxes:

    sudo apt-get libglew1.6-dev freeglut3-dev

### Building

    git clone https://github.com/danho123/ray-tracer
    cd ray-tracer/raycast
    make +d 5 +s +p +l
    ./raycast
