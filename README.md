# godgame

A cool game about designing shapes using mathematical formulas.

Algorithm explanation:

For an entered equation, the program samples the 3d points over a space, entering the 3d-coordinates as input x,y,z in the equation, the "x" in your equation corresponds to the x-coordinate in object space.
The entered equation is first compiled, then executed over the points, and then 8 convex shapes (2 in each direction so 2^3 = 8) are combined to create the output shape, bullet uses these to create physics for the object. The "Assembling" visuals is just a shader.

Dependencies: Freetype, Quickhull, TinyExpr, probably a few others.


Copyright 2018 Tobias Elinder

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
