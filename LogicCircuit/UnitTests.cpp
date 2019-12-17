#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <SFML/graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <fstream>
#include <vector>
#include <math.h>
#include <map>
#include <iostream>
#include <random>
#include <algorithm>
#include <memory>
#include "Explosion.h"
#include "MorphingVertex.h"
#include "Cube.h"
#include "Wind.h"
#include "Fog.h"
#include "Rain.h"
#include "ParticleRoom.h"
#include "IdAssigner.h"
#include "Component.h"
#include "Controller.h"
#include "Capsule.h"
#include "Receiver.h"
#include "LineConnection.h"
#include "Listener.h"
#include <windows.h>
#include <typeinfo>
#include "Gate.h"
#include "BitArray.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/pending/indirect_cmp.hpp>
#define WEST 0
#define NORTH 1
#define EAST 2
#define SOUTH 3

TEST_CASE("vectors can be sized and resized", "[vector]") {

    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SECTION("resizing bigger changes size and capacity") {
        v.resize(10);

        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= 10);
    }
    SECTION("resizing smaller changes size but not capacity") {
        v.resize(0);

        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 5);
    }
    SECTION("reserving bigger changes capacity but not size") {
        v.reserve(10);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 10);
    }
    SECTION("reserving smaller does not change size or capacity") {
        v.reserve(0);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
    }
}