# Spring Graph Layout
A spring graph layout algorithm written in C++ using Hooke's Law and Coloumb's Law to<br>
model force interactions, adjust constants in equations for different looking final graphs<br>

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
Takes user input and randomly generates a test files to be used for spring.cpp <br>
`Frames:`100<br>
`Nodes:`100<br>
`Edges:`2<br>
<br>

### Example output 100 nodes 100 frames 2 edges
![100 nodes](https://i.imgur.com/dSGJrZT.gif)

