# XY-Model
Simulation of XY-Model
# Usage
This code is parallelized with std c++, you may need to change the number of thread in the main.cpp, set the ```MAX_THREAD``` macro to the max logical threads of your CPU

if you want to save the simulation state, you may need to chage the output of the state folder at the very beginning of the main.cpp 
```string stateFileFolder = "c:\\XYModel_State\\";```

You can tweak the code so the final energy converge to the right value by checking the chage of the energy if it does converge

# Kosterlitz–Thouless transition
if you change the temperature from 20 to 0.1K, you will see the[Kosterlitz–Thouless transition](https://en.wikipedia.org/wiki/Kosterlitz%E2%80%93Thouless_transition). Here is a video [visualize the simulation](https://www.youtube.com/watch?v=vBzXrdGuMuU)

