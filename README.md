
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


