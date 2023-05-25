

rem compile all cpp files in engine directory
g++ -g -std=c++17 -I.\deps\include -c deps/include/jvk/*.cpp deps/include/obs/*.cpp

rem move all compiled files to one directory
move *.o compiled

pause