## jistat - clone of istat(1)

**Note:** Starting October 12 2023, Microsoft GitHub
is forcing me to use 2FA to access to my repositories.
Microsoft wants me to use my Cell Phone or some proprietary
application for 2FA, email is not allowed.  So updates to
this project will no longer occur on GitHub.  Updates and
full Source can be downloaded from my Gemini Capsule:

* gemini://gem.sdf.org/jmccue/computer/repository.gmi
* gemini://sdf.org/jmccue/computer/repository.gmi (mirror)

[See gemini wikipedia](https://en.wikipedia.org/wiki/Gemini_(protocol)#Software) for clients.

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
