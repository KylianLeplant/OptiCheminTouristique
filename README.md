# OptiChemin

## Why ?

-> interactive data visualisation with Raylib
-> cleaner implementation of the problem

## Run the project

```bash
xmake run
```

## Brainstorming

Other classes that may be necessary ?
-> HostelPath : represents a path of Hostels across the whole trip
-> POIPath : represents a path of POIs across single day.

Stuff still to do :

- Implement the solution class
- Add distance calculation functions
- Add score calculation functions
- Implement Kylian's changes for Hostel pathfinding

### Visualizer

I want the visualizer to do the following things

- Be decoupled from the main program. This means the program should work flawlessly without it.
- This means we may have to store optional logs for the generation, in order to be visualized back. It probably won't work in real time.
- The visualizer should show a view of the instance (and world map) with interactive hover-based information.
- The visualizer should handle a collection of instances to display, and allow switching back and forth with arrows of some kind
- The visualizer should show two kind of paths : the Hostels path and the POI paths. It should be able to show either one independantly, OR at the same time.
- The visualizer should show how the program solves the problem interactively. This can be done by storing each "frame" of the simulation in a log file (or a data structure) and playing it back.
- The visualizer should register what we want it to display beforehand, and then offer tabs and / or

The visualizer can have an array of "frame" arguments.
Each frame is a visualization and can be switched with the arrows.

A frame has some basic features, and can then be extended to create each type of visualisation.
