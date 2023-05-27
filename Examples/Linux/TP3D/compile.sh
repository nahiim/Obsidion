






# compile all cpp files in engine directory
g++ -g -Ideps/include -c deps/include/obs/*.cpp

# move all compiled files to one directory
mv *.o objs