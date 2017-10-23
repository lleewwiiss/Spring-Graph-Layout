# Spring Graph Layout
A spring graph layout algorithm written in C++ using Hooke's Law and Coloumb's Law to<br>
model force interactions<br>

## Spring
`Usage: spring test_file`<br>
<br>
Will output a series of frames which represent 
the node configuration at each interval. <br>

## Genvis
`Usage: genvis.py test_file`<br>
<br>
Takes the output files from Spring and converts them into a movie
file to visualise how the graph is moved over time<br>

## Gentest
`Usage: gentest.py`<br>
Takes user input and generates a test files to be used for spring.cpp <br>
`Frames:`<br>
`Nodes:`<br>
`Edges:`<br>
<br>

### Example output 100 nodes 100 frames 2 edges
![100 nodes](https://imgur.com/a/apvcH)

