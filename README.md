# GT_Assignment2

Assignment 3 Submission

Group Siren:
Gahwon Lee
Yijin Zhao
Amber Smith
Clinton Bell


How to Build and Run:
build with command scons p=<platform>
run in godot editor (need separate editors open for server and client)
for server, press the create server button
for clients, press the join server button and input the server's ip address (find with ifconfig on linux, or use localhost when running on the same machine as the server) to join
collect acorns together before the time runs out


Notes:
We heavily referenced this repository as a base for our networking code: https://github.com/devonh/Godot-engine-tutorial-demos/tree/master/2018/07-30-2018-multiplayer-high-level-api
If a client disconnects, other clients and the server continue to work, but on server disconnect, all clients can no longer play and must quit.
