## jistat - clone of istat(1)

**Note:** Starting October 12 2023, GitHub is forcing me to use
my Cell Phone for 2FA to allow access to my repositories.  I do
not want Microsoft to have access to my phone.  So updates to
this project will no longer occur on GitHub.  I will make a note
of where source can be retrieved from on my Gemini Capsule once
everything is moved off of GitHub.
Capsule: gemini://gem.sdf.org/jmccue/ or mirror gemini://sdf.org/jmccue/

jistat is a clone of istat (or stat(1)) for systems that
does not have either.

This is useful if you need a 'standard default'
print of File Information.
istat(1) or stat(1) prints differ depending upon the
Operating System.

[j\_lib2](https://github.com/jmcunx/j_lib2) is an **optional** dependency.

[GNU automake](https://en.wikipedia.org/wiki/Automake)
only confuses me, but this seems to be good enough for me.

**To compile:**
* If "DESTDIR" is not set, will install under /usr/local
* Execute ./build.sh to create a Makefile
* Works on Linux, BSD and AIX

_To uninstall_, execute
"make uninstall"
from the source directory
