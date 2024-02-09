# Homework-03: Scientific Computing Toolbox
This is the project for Homework-03 of the Advanced Programming course.  
The following file is the same README.md from the Homework-02, with additional info from the Python part of the project.

## Modules
We chose to implement Module A and Module C of the Homework-02.

## Build and Run
### Building the Python modules
To build each module, a CMakeLists.txt file in the corresponding python/ folder is provided. Run the commands  
```
mkdir build && cd build
cmake ..
make
``` 
Libraries muParserX and Boost are required and their location can be specified using  
```
-DMUPARSERX_ROOT=/path/to/muparserx/root
```  
and  
```
-DBOOST_ROOT=/path/to/boost/root
``` 
The required pybind11 location can also be specified using  
```
-DPYTHON_INCLUDE_DIRS=/path/to/python/installation/libraries
```  
We also had to run the command  
```
export CPLUS_INCLUDE_PATH=/path/to/python/installation
```  
beforehand, so that pybind would find the Python.h file, but this is not required if this path is already set in the system variable.  
A main.py file is provided for each module, as an example of the offered functionalities. Run the file using
```
python3 main.py
```
For the module A, the main.py file will run a demo on default input and output files, unless their paths are provided as command line arguments.  
For the module C, the command line argument "demo" can be provided, so that the main.py files runs a little demo of all the functionalities.  
### Building the c++ main files
The build.sh files expect to find libraries boost and muparserx inside /urs/local.
To build each module's example, run the corresponding build.sh and then run the program with ./main

## Group
Our group consists of Camilla Giaccari (camillagiaccari97@gmail.com) and Lorenzo Giaccari (lorenzo.giaccari99@gmail.com).
We worked on the same machine with a pair programming approach, frequently swapping between who was writing and who was checking the code.

## Code organization
- SCToolbox/
    - Statistics/ (module A)
        - csv/
            - df_surf.csv
        - src/
            - main.cpp (contains some tests)
            - statistics.cpp
        - include/
            - fifo_map.hpp (header-only library)
            - statistics.hpp
        - python/
            - main.py (contains some tests)
            - statistics_py.cpp (pybind11 binding)
            - statistics.py (additional python functionalities)
            - CMakeLists.txt (to build Python module)
        - build.sh (to build c++ examples)
    - Numerical_integration/ (module C)
        - src/
            - main.cpp (contains some tests)
            - approximator.cpp (abstract base class)
            - midpoint_approximator.cpp
            - simpson_approximator.cpp
            - trapezoidal_approximator.cpp
        - include/
            - approximator.hpp (abstract base class)
            - midpoint_approximator.hpp
            - simpson_approximator.hpp
            - trapezoidal_approximator.hpp
        - python/
            - main.py (contains some tests)
            - approximator_py.cpp (pybind11 binding)
            - CMakeLists.txt (to build Python module)
        - build.sh (to build c++ examples)
    - README.md

## Design choices and performance discussion
### Python integration (Homework-03)
We decided to put everything regarding the Python integration inside a python/ folder for each module.  
The binding part of the code is in a separated file named after the module (with _py.cpp).  
#### Module A:
We made the binding for all the Statistics class methods and added an iterator using \_\_iter\_\_, which uses our begin() and end() methods.  
We also binded the \_\_repr\_\_ magic method with a lambda function: using the Statistics print() method, it redirects the output stream to a string, that is then returned as an output. This way we can print the data with the Python print(), in addition to our Statistics::print() method.
In the file statistics.py, we decided to add some Python-only functionalities: inside a child class of the one resulting from the binding, we implemented a timer decorator, a timed version of all the methods from the parent class, the utility method get_col, a Python-only version of mean and write_mean and two plotting methods. The user will then include this child class instead of the binded one.  
To visualize the data, we decided to use a column histogram, so that the frequencies of all the data values are visible; we also added an overview of all the statistics calculations that can be made on a single column. This plot can be made for categorical-valued columns too, but without the statistics calculations. The user just needs to provide the name of the column to plot everything.  
In addition, we provide a scatter plot to better visualize the correlation between two columns, with a red line to represent a perfect correlation of 1. This time the plot is reserved for numerical-valued columns, using a check that raises a ValueError exception instead. The user just needs to provide the names of the two columns, but with caution regarding the aforementioned exception.  
During development, we considered replacing the constructor of Statistics, so that we could load the data in a Pandas DataFrame, but we ran into some issues: first, if the data was to be stored in a DataFrame, none of the c++ methods of Statistics would have been usable (they work on a fifo_map), forcing us to rewrite all of them in Python (defeating the idea of an hybrid approach) or to create an utility method to convert a DataFrame into a fifo_map and vice-versa (which would have impacted very negatively on the time performance). Furthermore, some early tests confirmed our hypothesis that even the simple import of the Pandas module was enough to delay the execution of our whole main program.    
So we decided to maintain the c++ constructor with the old data-loading and focus on the peculiarities of Python, using a decorator and plotting some graphs with ease.  
The main.py also contains a little test demonstrating the validity of the hybrid approach over the Python-only one, with respect to the execution time.
#### Module C:
We made the binding for the Approximator class and one for each of its child classes. We had to use a Trampoline c++ class, since Approximator is virtual and we have to make sure Python will not try to instantiate it.  
When writing the main.py file, we realized how much easier it was with respect to the main.cpp one, making the hybrid approach worth the time. In particular, the else block at the end of the file, which required a lot of tricks regarding the cleaning of the input in c++, was very linear to write in python, and is also a lot more readable.
### C++ library (Homework-02)
We decided to organize each module in its own directory, but they both share the same namespace 'sctoolbox'.  
All functionalities of Module A are inside a single class, with declaration and definition of the methods separated in two different files. Module C is organized into three different classes, each one implementing a different rule and deriving from the same abstract class. Declaration and definition of the methods are separated in two different files here too.  
#### Module A:
The user can provide the Statistics constructor with valid paths to an input csv file and an output file or use the default ones. If it doesn't exist, the output file is created automatically at the first printing. The csv file is expected to have the name of the columns as the first row.  
Each function of statistics has two versions, one that returns the result (es. 'mean') and one that puts this result into the specified output file (es. 'write_mean').  
We implemented controls on the provided column name and on the type expected for the elements, throwing exceptions in case something is wrong. Missing values are identified as the empty string "" and so the methods handle them as needed, without throwing exceptions.  
We decided to store our data as a fifo_map, since we load from the file first the keys and then the values, and an std::map would rearrange alphabetically the keys, skewing everything.
We tested our application using the df_surf.csv file from https://www.kaggle.com/datasets/loureiro85/surfing/.
#### Module C:
The methods work with any valid mathematical expression, and will throw an exception on non-valid ones.  
We heavily used the functionalities provided by the muParserX library.  
The user will call the approximate() method from the base class, and its implementation will change at runtime depending on the constructed deriving class. These classes provide the overridden method approximate_step() as private, since we don't expect the user to call it.  
The main file provides a way to calculate an integral provided by the user; when run using "./main demo", it prints a little demo with some pre-written examples.

## Libraries
### Python modules
- pybind11, to integrate Python and c++ code
- time, to create the timer decorator
- numpy, to compare its mean method
- matplotlib.pyplot and seaborn, to plot
- os, to check the working directory
- sys, to deal with user inputs
- itertools, to recover a single element of an iterable through its index
### C++ libraries
Boost, muParserX and fifo-map libraries are used in this project.  
We had to use three different approaches when building and linking them, which was a bit time-consuming, but not excessively difficult. Boost required a single make command, fifo-map, as a header only library, just needed to be included in the project, and muParserX had to be built and installed using cmake.  

## Links
### Python integration (Homework-03)
- CMakeLists.txt lines for Boost linking found at https://stackoverflow.com/questions/10225720/making-boost-work-with-cmake-fatal-error-boost-smart-ptr-hpp-no-such-file-or
- cmake version required by Boost found at https://stackoverflow.com/questions/42123509/cmake-finds-boost-but-the-imported-targets-not-available-for-boost-version
- solved "Python.h file not found" error while building thanks to https://stackoverflow.com/questions/35778495/fatal-error-python-h-file-not-found-while-installing-opencv
- pybind11 trampoline info found in the documentation at https://pybind11.readthedocs.io/en/stable/advanced/classes.html?highlight=trampoline#forced-trampoline-class-initialisation
- in \_\_repr\_\_ binding in Statistics, we redirect c++ standard output to string as seen from https://stackoverflow.com/questions/4810516/c-redirecting-stdout 
- we bind the Statistics iterator as done in https://stackoverflow.com/questions/62347521/using-pybind11-to-wrap-yaml-cpp-iterator
- we found a way to get the specified element from an iterator at https://stackoverflow.com/questions/2300756/get-the-nth-item-of-a-generator-in-python
- We referred to the documentation for plt.axline https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.axline.html#matplotlib.axes.Axes.axline
### C++ (Homework-02)
- the header only library fifo-map can be found at https://github.com/nlohmann/fifo_map.
- we found an useful check for double at https://stackoverflow.com/questions/29169153/how-do-i-verify-a-string-is-valid-double-even-if-it-has-a-point-in-it.
- we used the method 'visit' to print a variant as found at https://stackoverflow.com/questions/62707343/how-can-i-print-map-key-value-with-stdvariant.
- we skip missing elements in the correlation evaluation, as described in the "Complete Case Analysis" at https://dept.stat.lsa.umich.edu/~kshedden/Python-Workshop/correlation_missing_data.html#complete-case-analysis
- we found a way to assure user inputs as double at https://stackoverflow.com/questions/3273993/how-do-i-validate-user-input-as-a-double-in-c.

## Licenses
### Boost:  
Boost Software License - Version 1.0 - August 17th, 2003  
Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:  
The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.  
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.  
### muParserX:  
BSD 2-Clause License  
Copyright (c) 2023, Ingo Berg  
All rights reserved.  
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:  
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.  
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
### fifo-map:  
MIT License  
Copyright (c) 2015-2017 Niels Lohmann  
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.  
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
