# Project: Mandelzoom
A Mandelbrot set viewer.

### Features:
- Uses rubber band to zoom into a particular region.
- Dynamically resizes the window size.
- Undo/redo previous zooms.
- Randomize colors with the <kbd>space</kbd> key.

### To-do's (in order of decreasing priority)

- Better formula so that the set appears good regardless of each zoom level.
  - Somehow relate intensity to zoom levels? That is, there is some relation between the distribution of intensity and the zoom levels, e.g. at shallower levels, most points are of low intensity (they hardly escape). But there is always subjective effect on zooming: we select the more interesting parts, i.e. the ones with less all-black or all-white points and more gradual gradient, to zoom in. This suggests that re-calibrating the intensity should be local to each zoom level, not altering the global function which maps intensity to a color. For instance, assuming that we start with the darkest image (the distribution of points is heavily skewed toward low intensity, we can multiply the intensity by some factor (linear? non-linear?) so the sub-region appears more normal (intuition tells me that we should dim the zoomed-in region, but experimentally I obtain higher values for the color parameters in deeper zooms; Of course I do not fully understand how the color parameters are used by the color mapping function that I stumbled across.)

- Make a Makefile that different OSes (Windows, macOS and Linux).
  
- (Need testing) Redesign reshape strategy to preserve the center. (Or fully fix reshaping -- only render what is absolutely need to be re-rendered.)
  - Might be trivial: just rescale with a factor of 1.0.

- Experiment with the idea of whether it is efficient to save every picture drawn, and to keep a copy of the previous drawn ones to enable faster zooming out.

- √ Fix drawing rubber band, which sometimes disappear (seems to non-deterministic). (*Update*: I forgot to set rubber band drawing color to white every time.)

- Better zooming animations: fading out the entire image, or gently fading out the advancing edge.

- Implement rubber-banding using laryers. Can we access the pixels in a particular layer (same layer, different layer?) We can certainly do xor, and other simple logic operations.

- Speed things up.
  - Remove expensive operations:
    - √ Less multiplication, sqrt, etc.
    - Less dependency to enable for loop unrolling.
    - Use faster approximation formula for sine (?)
  - Cross-OS multi-threading in color mapping.

- Make `Point` a superclass of `Complex` and `Pixel` .

- Automatic zooming into a selected point.
  - Would need better precision (infinite precision?) floating point arithmetic.

- √ Fix probable memory leaks.

- √ Fix redo/undo to support further zooming-in/out even when the stack of operations is empty.

- √ Separate out the code that maps intensity values to RGB triples into a standalone `Color` class.
  - Linearize distribution of intensity (I), defined as the ratio between the number of trials before the sequence diverge and the cap.
  - Better mapping from I to RGB triples ([0,1] -> [0.1]^3).
    - Currently using sine with period of `1 + alpha`, where `alpha` is a paramter.
  - Design some animation to traverse different combinations of RGB triples, ~~possibly via the inverse Cantor pairing function generalized to Z^3.~~ (*Update:* implemented the inverse of Cantor pairing in 3D, but found out that it was not a good way to traverse the triples. The elements at the front vary substantially more than later elements. Intuition is that when the function produces a pair, the first element has exclusive access to the first of the pair, but the tail elements must share the second of the pair. Thus distribution is not smooth when transitioning from one triple to the next.)
  - A larger zoo of non-linear functions and ways of combining them.
    - Stumbled upon the intermediate function `tan(1/log(intensity+1))/PI*3` which works reasonably when piped into the function `sin(I*2PI * (k/2 +1))`.