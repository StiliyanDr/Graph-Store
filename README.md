# Graph store
A command line application written in C++ that lets you build graphs and apply operations to them, including searching for shortest paths.

## Table of contents
* [Introduction](#introduction)
* [Commands](#commands)
* [Text file format](#text-file-format)
* [Example](#example)
* [License](#license)
* [Other information](#other-information)

## Introduction
The application lets you build undirected and directed graphs that can also be weighted. Edge weights in weighted graphs must
be natural numbers. The most important operation that can be done with graphs is a search for a shortest path between two vertices.
Graphs can be stored on disk as text files using a [simple format](#text-file-format). Such files can also be supplied by users and
are used by the application for building graphs described in them.

When started, the program must be given the path of a directory which is to be its working directory. This directory may contain
text files (as described above). During runtime the application maintains a **collection of graphs** which is initialised with the
graphs built from the text files in the working directory (if any). At any time during program execution one of the graphs from
the collection may be selected as the **used graph** - an implicit argument to some of the [commands](#commands).

Graphs and vertices have identifiers (**ids**). Any nonempty string is a valid graph and vertex id.  
No two graphs in the collection can have the same id, vertices in a graph must also have different ids.

This project was initially an assignment for the Data Structures and Algorithms course at my university. I later used it
as a place to implement a few graph algorithms that were part of the Design and Analysis of Algorithms course and to get
familiar with programming practices discussed in a Clean Code course. Now I maintain it in my free time and use it to practice C++.

## Commands

By default, words in the command line are delimited by spaces.  
To group a sequence of characters in a single word you can also use single quotation marks (') and double quotation marks ("),
with the following limitations:
 - delimiters must match, that is, the start and the end of the word must be marked with the same delimiter
 - delimiters can't be escaped

Notations:
When showing a command syntax, parameters will be surrounded by **<** and **>**.  
That is, if *c* is a command with parameters *p* and *q*, its syntax is:  
c \<p\> \<q\>

If a parameter is optional, it will also be surrounded by **[** and **]**.  
In the above example, if *q* was optional, the syntax of *c* would be:  
c \<p\> [\<q\>]

Supported commands:
* [help](#help)
* [list-graphs](#list-graphs)
* [use-graph](#use-graph)
* [print-graph](#print-graph)
* [add-graph](#add-graph)
* [remove-graph](#remove-graph)
* [add-vertex](#add-vertex)
* [remove-vertex](#remove-vertex)
* [add-edge](#add-edge)
* [remove-edge](#remove-edge)
* [search](#search)
* [save](#save)
* [exit](#exit)

### help
This command takes no arguments.  
It prints the names of all supported commands.  
A brief description of each command is given next to its name.

### list-graphs
This command takes no arguments.  
It prints the ids of all the graphs in the collection, each on a separate line.

### use-graph
Syntax: use-graph \<id\>

If there is a graph with id \<id\>, it is set as the used one.  
Otherwise an error message is printed to standard error.

### print-graph
Syntax: print-graph [\<id\>]

Prints the graph with id \<id\> in the following way.  
For each vertex v of the graph a single line is printed, it has the form:  
v: (w₁, u₁), ..., (wₖ, uₖ)  
where u₁, ..., uₖ are all the vertices adjacent to v and w₁, ..., wₖ are the weights of the corresponding edges.

If \<id\> is not specified, the id of the used graph is assumed.  

If there is no graph with id \<id\> or \<id\> is not specified and no graph is used, an error message is printed to standard error.

### add-graph
Syntax: add-graph \<id\> [\<type\>]

Adds a new (empty) graph with id \<id\> and type \<type\> to the collection.  
\<type\> may be either **undirected** or **directed** but if it is not specified, undirected is assumed.

If at least one of \<id\> and \<type\> is invalid or  there already is a graph with such an id in the collection, an error message is printed to standard error.

### remove-graph
Syntax: remove-graph \<id\>

If there is a graph with the specified id in the collection, it is removed.
Its corresponding file in the working directory (if any) is removed too.  
Otherwise an error message is printed to standard error.

### add-vertex
Syntax: add-vertex \<id\>

Adds a new vertex with the specified id to the used graph.  
If either no graph is used, the id is invalid or another vertex in the graph has the specified id, an error message
is printed to standard error.

### remove-vertex
Syntax: remove-vertex \<id\>

Removes the vertex with the specified id from the used graph.  
If no graph is used or no vertex has a matching id, an error message is printed to standard error.

### add-edge
Syntax: add-edge \<start id\> \<end id\> [\<weight\>]

Adds an edge with weight \<weight\> from the vertex with id \<start id\> to the vertex with id  \<end id\>
in the used graph. Note that the edge is directed if and only if the graph is directed.

When \<weight\> is not specified, it is assumed to be 1. This is the way to add an edge to an unweighted graph.

Loops are not allowed in undirected graphs and two vertices can not be connected by more than one edge
(with the same direction).

If either of the following conditions is met, an error message is printed to standard error:
- no graph is used  
- at least one of the arguments is invalid  
- at least one of the specified ids matches no id of a vertex in the graph  
- the graph is undirected and the proposed edge is a loop  
- there already is such an edge in the graph  

### remove-edge
Syntax: remove-edge \<start id\> \<end id\>

Removes the edge from the vertex with id \<start id\> to the vertex with id \<end id\> in the used graph.

If no graph is used, at least one of the specified id's does not match an id of a vertex in the graph or
there is no such edge in the graph, an error message is printed to standard error.

### search
Syntax: search \<source id\> \<target id\> \<algorithm id\>

Finds and prints a shortest path from the vertex with id \<source id\> to the vertex with id \<target id\>
in the used graph using the algorithm specified by \<algorithm id\>.

If such a shortest path exists, its length and the ids of the vertices constituting the path are printed,
starting from the source. The length given in the result is the sum of the weights of the edges constituting
the path, if the graph is weighted, otherwise it's the number of the edges.  
If no shortest path exists, an appropriate message is printed.

The following algorithms may be used for the search, the corresponding algorithm id is given: 
bfs, dijkstra, dfs-shortest (for iterative deepening dfs).

If either no graph is used, at least one of \<source id\> and \<target id\> does not match an id of a vertex
in the used graph or \<algorithm id\> does not match an id of one of the supported algorithms, an error message
is printed to standard error.

### save
This command takes no arguments.  
It saves every graph from the collection in the working directory as a text file using the format specified
[here](#text-file-format).  
The name of the file corresponding to a graph with id **graphID** is **graphID.txt**.  
If a file with such a name already exists, its contents will be overwritten.

### exit
This command takes no arguments.  
It simply exits the program. It does not save any changes, nor does it check if any were made.
To save any changes you made, you have to use the [save](#save) command.

## Text file format
The following format is used to store graphs as text files on disk:

\<graph id\>  
\<graph type\>  
\<number of vertices\>  
\<vertex 0 id\>  
...  
\<vertex (n - 1) id\>  
\<number of edges\>  
\<edge 0\>  
...  
\<edge (m - 1)\>  

Where:
- \<graph id\> and \<vertex i id\> are valid graph and vertex ids respectively
- \<graph type\> is either **directed** or **undirected**
- \<number of vertices\> and \<number of edges\> are natural numbers
- \<edge k\> has the form: **(i, j, w)**  
   **i**, **j** and **w** are natural numbers, 0 ≦ **i**, **j**  < n.  
   **i** and **j** correspond to the vertices with ids \<vertex i id\> and \<vertex j id\> respectively.  
   **w** is the weight of the edge.  
   If the graph is directed, the edge is from \<vertex i id\> to \<vertex j id\>.

Spaces are ignored in lines for edges, number of vertices and number of edges.  
In lines for ids and type, however, spaces are not ignored. That is, the whole line is used.

## Example

Suppose you want to supply files for two graphs *G1* and *G2* which look like this, respectively:
![Example graphs](https://github.com/StiliyanDr/Graph-Store/blob/master/Example%20graph%20files/graphs.jpg)

I have provided two example files for them:
the one for *G1* you can find
[here](https://github.com/StiliyanDr/Graph-Store/blob/master/Example%20graph%20files/G1.txt),
and the one for *G2* -
[here](https://github.com/StiliyanDr/Graph-Store/blob/master/Example%20graph%20files/G2.txt).

You simply put the files in the directory which is to be the working directory.  
Note that the working directory may contain no text files at all, they are optional.  
The only argument you need to pass to the program when starting it is the path of the directory.  

Here is a very simple example use of the program, once started:

$ list-graphs  
G1  
G2  
$ use-graph G2  
$ print-graph  
one: (56, three), (20, two),  
two:  
three: (18, four),  
four: (2, five), (43, two),  
five: (36, six),  
six:  
$ add-edge three six 2  
$ add-vertex seven  
$ add-edge six seven  
$ search one seven dijkstra  
Path: one, three, six, seven,  
Path length: 59  
$ save  
$ exit

## License

## Other information
The command line parsing part of the project uses a library which you can find and learn more about [here](https://github.com/Taywee/args).
