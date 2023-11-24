I attempted to complete the CMakeLists statement, but encountered a very strange problem. 
Previously, I followed the approach in the PDF to add all source files into a string variable named XXX_FILE using the set command. 

However, this time it failed. 
```
CMake Error at quantize/CMakeLists.txt:6 (add_library):
  Cannot find source file:

    *.c

  Tried extensions .c .C .c++ .cc .cpp .cxx .cu .mpp .m .M .mm .ixx .cppm .h
  .hh .h++ .hm .hpp .hxx .in .txx .f .F .for .f77 .f90 .f95 .f03 .hip .ispc


CMake Error at quantize/CMakeLists.txt:6 (add_library):
  No SOURCES given to target: Source_lib
```
Eventually, after researching other methods to include all files with '.c' and '.h' extensions, I was able to resolve the issue.
```
file(GLOB QUANTIZE_SOURCES "*.c")
file(GLOB QUANTIZE_HEADERS "*.h")
```

I ran the generated program, and the file size of the resulting image was reduced compared to the original image.
