## jistat - clone of istat(1)

jistat is a clone of istat (or stat(1)) for systems that
does not have either.

This is useful if you need a 'standard default'
print of File Information.
istat(1) or stat(1) prints differ depending upon the
Operating System.

To build, execute build.sh to generate a Makefile from
Makefile.template on most BSD/Linux systems and IBM AIX.
For MS-DOS, it should be rather easy to create a Makefile.

This requires [j\_lib2](https://github.com/jmcunx/j_lib2) to build.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, so I came up with my own method which
is a real hack.  But works for me.
