# CDraw
A basic drawing library for C based off the one used in [HtDP](https://docs.racket-lang.org/teachpack/2htdpimage.html)

Basically this is a C port of a library designed for Lisp, rewritten from the ground up. The library is simple enough that it already has been ported to multiple graphics backends. The following backends are currently supported:

* Cairo
* Qt 5/6

The Qt port probably also runs on Qt4 and maybe earlier, further testing is required.

For a project written using CDraw, please see [Nematode](https://github.com/bravotic/nematode)
