## jistat - clone of istat(1)

jistat is a clone of istat (or stat(1)) for systems that
does not have either.

This is useful if you need a 'standard default'
print of File Information.
istat(1) or stat(1) prints differ depending upon the
Operating System.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX

_To uninstall_, execute
"make uninstall"
from the source directory
