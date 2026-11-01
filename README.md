# Introduction
A basic web server written in C++. I created this project to learn about web server and how they actually work under the hood.
Only works on linux

# Building
```
mkdir build
cd build
cmake ..
make -j12
```
This will generate a ```webserver``` executable in the project root directory
# Running
In the project root directory, run
```
./webserver
```
Open a web browser and goto http://localhost:8080
# Features
* Handling GET request.
