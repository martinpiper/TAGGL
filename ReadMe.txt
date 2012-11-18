Source code to render TBA Software's TAG data files.

main.cpp : init()
This can load files from the command line or be used to merge several systems together.

If the command line mode is enabled then passing in "Data\TestAnim1" should load and run the Holograph TestAnim1 example.
This sample displays some cubes that should spin and bounce around inside a larger cube. But the collision detection code has not been completed yet so the cube handlers will not get the events. So what happens is that most of the cubes will sink through the floor.

The eventual goal is to be able to load BHP data files and see the game work. But this will require quite a lot of work to get all the supported handler and engine functionality working.


Some interesting configuration options to remember in the code:
bool ARMCore::mDebuggingEnabled
bool ARMCore::mDisassemble
