# C++ Scalable Vector Graphics utilities library  .version 1.0

## How to use/build
**Note: all following instructions are designed for linux system, if you are using other systems, please only use them as an reference, as the process might be simular**
### Viewer
1. Make sure you have `sfml`(gui support), `tinyxml2`(raw info extract) and a proper c++ compiler installed.
2. To build, run `bash make.bash` in terminal in the root of the project folder.
   
   **Note: designed for gcc compiler only, please write your own if you are using otherwise**
3. Expect an executable be produced in the root of project folder named `svgviewer`

   To use, run `./svgviewer <path/to/your/file.svg>`, then expect an gui.
   
   example screen shot: `./svgviewer ./svgs/flower.svg`
   ![screen shot of gui](https://github.com/user-attachments/assets/1fe39442-c1b4-4fcf-9eec-411c89976430)
Gui explaination:
   - Right: graphic extracted from svg.
   - Left: point graph produced through function:
   
     `sl::pointgraph sl::simplify(pointgraph old,double minLen,double maxLen,double minAngle)`
### SVG Utility library
1. Make sure you have `tinyxml2` installed, used for extract raw info from svg file, which is also part of xml family.
2. Add `svglib.hpp` and `svglib.cpp` together to the folder you want (make sure they are in the same folder)
3. Add `#inlcude <path/to/svglib.hpp>` to your program file where you are going to implement it
   
   or just `#inlcude "svglib.hpp"` if they are in the same folder with your program file
4. As you can see in `svglib.hpp`, all utility functions are stored in namespace `sl`, import the namespace and start coding XD!

## File system explain
### Sources files - "src/"
- `svglib.hpp`/`svglib.cpp`
  Includs all svg utilities provided, tested, and useable.
- `main.cpp`/`view.hpp`/`view.cpp`:
  A simple sfml program for displaying utilty results for testing purposes.
- `svgtran.hpp`/`svgtran.cpp`:
  Utilities for svg's transform attribute. **Note: not implemented**
### SVG files - "svgs/"
A list of simple svg file collected that this library supports and can work with for testing purposes.
### Others
- `make.bash`:
  Viewer program building command script for bash.
  
  To use, run `bash make.bash` in terminal in the root of the project folder.
- `svgviewer`:
  Linux executable for viewer program.

  To use, run `./svgviewer <path/to/your/file.svg>`, then expect an gui.
- `LICENCE.txt`: GPL Licence
## Note.
1. As explained, all [Transform attribute](https://www.w3schools.com/graphics/svg_transformations.asp) are unfortunaly not implemented.

   In simple terms: I tried, and I failed...
2. For anyone who is willing to suggest/edit/improve on source code, I am more than wellcome
3. This project is GPL licenced.
