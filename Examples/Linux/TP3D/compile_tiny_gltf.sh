






# compile all cpp files in engine directory
g++ -g -c deps/include/tiny_gltf/tiny_gltf.cc 

# move all compiled files to one directory
mv *.o objs		