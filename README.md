# Project: Mandelzoom
A Mandelbrot set viewer.

### Features:
- Uses rubber band to zoom into a particular region.
- Dynamically resizes the window's aspect ratio.
- Undo/redo previous zooms.

### To-do's (in order of decreasing priority)
- ☑️ Separate out the code that maps intensity values to RGB triples into a standalone `Color` class.
  - Linearize distribution of intensity (I), defined as the ratio between the number of trials before the sequence diverge and the cap.
  - Better mapping from I to RGB triples ([0,1] -> [0.1]^3).
    - Currently using sine with period of `1 + alpha`, where `alpha` is a paramter.
  - Design some animation to traverse different combinations of RGB triples, ~~possibly via the inverse Cantor pairing function generalized to Z^3.~~ (*Update:* implemented the inverse of Cantor pairing in 3D, but found out that it was not a good way to traverse the triples. The elements at the front vary substantially more than later elements. Intuition is that when the function produces a pair, the first element has exclusive access to the first of the pair, but the tail elements must share the second of the pair. Thus distribution is not smooth when transitioning from one triple to the next.)
  - ☑️ A larger zoo of non-linear functions and ways of combining them.
    - Stumbled upon `tan(1/log(intensity+1))/PI*3` which works reasonably well combined with the triple `(0.71,0.32,0.29)` mapped by the function `sin(I*2PI * (k/2 +1))`. Gives an outer-space feeling.
- Fix drawing rubber band, which sometimes disappear (seems to non-deterministic).
- Fully fix reshaping -- only render what is absolutely need to be re-rendered.
- Speed things up.
  - Remove expensive operations:
    - Less multiplication, sqrt, etc.
    - Less dependency to enable for loop unrolling.
    - Use faster approximation formula for sine.
  - Cross-OS multi-threading in color mapping.
- Make `Point` a superclass of `Complex` and `Pixel` .
- Automatic zooming into a selected point.
  - Would need better precision (infinite precision?) floating point arithmetic.
- ☑️ Fix probable memory leaks.
- ☑️ Fix redo/undo to support further zooming-in/out even when the stack of operations is empty.