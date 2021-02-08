# Graphics
Coursework from CMSI 371: Graphics completed at LMU

# Assignment #1: Cartoonify
I used Bezier's algorithm to draw multiple curves that will form an image. A user is able to input points that will be used in the algorithm to transform points to a line and finally, into a curve. I inputted points for an image of my friend, and found that Bezier's curve algorithm rendered the points better than Chaikin's algorithm would have, since Chaikin's algorithm does not keep endpoints in its iterations.

In the code I created two functions one that would generate midpoints and generate points. In addition, I made sure to put the control points, mid points, and mid-mid points in a vector then I would interleave each of the mid points and mid-mid points and use the first control point then interleave midpoints and put the last control points when generating my line. When drawing the curve I iterated to each of the points. The way I chose my points, I inputted the image in Desmos and created the points with the help of a grid. Lastly, after picking all of my points I call all of the draw curve functions and it prints out the image.

Coded in C++ on Xcode using openGL and GLUT frameworks.
Original Image | Final Image
-------|---------------
![Alt Text](OriginalPic.jpg)   | ![Alt Text](Results.png)



# Assignment #2: 3D Model
This assignment uses rotation matrices around the x, y, and z axes as well as a scaling and translation matrix to create objects in a scene. This begins with a rendering of a cube, created by making one plane, and duplicating that plane by rotating and translating to get all six cube faces.

In this project I added color and implemented cross product, dot product, normals, and etc. I implemented and added two functions that subtract vectors and calculate the height. I utilize those helper functions in my apply shading function. In my apply shading function I declared variable and used a for loop to implement.

Coded in C++ on Xcode using openGL and GLUT frameworks.
![Alt Text](3DModel.gif)
