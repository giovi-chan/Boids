# Boids
This C++ project simulates a flock of flying boids, mimicking realistic group behavior and dynamics.

-----
to build in release mode, as is advised for better performance:

```
cmake -S ./ -B build/release -DBUILD_TESTING=True -DCMAKE_BUILD_TYPE=Release
```

and then

```
cmake --build build/release
```

to run the simulation:

```
build/release/Boids
```

to run the test for the program:

```
build/release/Boids.t
```

-----
To build in debug mode use instead:

```
cmake -S ./ -B build/debug -DBUILD_TESTING=True -DCMAKE_BUILD_TYPE=Debug
```

and then

```
cmake --build build/debug
```

to run the simulation:

```
build/debug/Boids
```

to run the test for the program:

```
build/debug/Boids.t
```

-----
one can also set

```
-DBUILD_TESTING=False
```

to not build the tests
