
Inconference Library
====================

Description
-----------

Intention of this library is to solve problem of scalable data delivery to multiple endpoints with different 
network channel bandwith. 

Given
    Number of endpoints.
    List of network channel bandwith values for each endpoint. 
    List of pre-processors for each endpoint.
When 
    Data is sent by one endpoint 
Then
    Data is received by all endpoints subscribed for it
    Data is pre-processed before it is sent to endpoint


Features
--------

*   Designed with scalability in mind 
*   Shared memory buffers with auto-distribution 
*   Tunable chain of pre-processors and interceptors 


How to run Linux based DEV ENV 
-------------------------------

Go to 'devcontainer' folder and build docker container with commond ```docker build . -t dev_iconf```

Then return to project directory and execute to build update Makefile and make project: 

```docker run --rm -it -v "$PWD":/proj -w /proj dev_iconf    cmake . && make```


Execute next command to tests: 

```
    docker run --rm -it -v "$PWD":/proj -w /proj -e LD_LIBRARY_PATH=/proj/src dev_iconf    \
        ./test/inconferenceTests --help
```

Execute next command to make: 
```
    docker run --rm -it -v "$PWD":/proj -w /proj -e LD_LIBRARY_PATH=/proj/src dev_iconf   \
        make
```

Execute next command to run valgrind:
```
    docker run --rm -it -v "$PWD":/proj -w /proj -e LD_LIBRARY_PATH=/proj/src dev_iconf   \
        valgrind ./test/inconferenceTests 
```
