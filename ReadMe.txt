Source code to render TBA Software's TAG data files.
It is a Windows application but most of the code is cross platform. It uses OpenGL to render. As is common with most of my projects it also reuses some of the ReplicaNet amalgamated source library from www.replicanet.com
Because it's a Windows application it needs to be able to emaulate the ARM, so it uses Wei Qin's ARM emulator code with some tweaks to allow 26 Bit ARM emulation.

The eventual goal is to be able to load BHP data files and see the game work. But this will require quite a lot of work to get all the supported handler and engine functionality working.




The code starts at WinMain and then gets to here:
main.cpp : init()
This can load files from the command line or be used to merge several systems together.

If the command line mode is enabled then passing in "Data\TestAnim1" should load and run the Holograph TestAnim1 example.
This sample displays some cubes that should spin and bounce around inside a larger cube. But the collision detection code has not been completed yet so the cube handlers will not get the events. So what happens is that most of the cubes will sink through the floor.




Some interesting configuration options to remember in the code:
bool ARMCore::mDebuggingEnabled
bool ARMCore::mDisassemble
