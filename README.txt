There are two different versions of this file on Canvas!
----------------------------------------------------------
If you're compiling on a Mac, on CAEN, in VS Code (on any
Operating System), or inside of the WSL (Windows Subsystem
for Linux) on a PC, use the archive named like that.

If you're running INSIDE of Visual Studio (NOT VS Code),
use the archive named that way.

If you're on a PC and using both Visual Studio and the
WSL, you will need both archives, unpacked into two
different folders.

This is all because Windows indicates an end-of-line
character different from the rest of the world, and using
getline() breaks if you are using the wrong type of input
file.
----------------------------------------------------------
The files spec-L.txt and spec-M.txt are the first input
files from the project specification; spec-pipe-L.txt and
spec-pipe-M.txt are the input files from the Appendix that
require you to use a pipe to find the Countess.

The Sample-Small and Sample-Big input files were generated
by the same program that created most of the input files
used by the autograder.

The output file names indicate the flags used (sl is stack
mode, list output, etc.).

The small input files took about 0.01 seconds or less to
complete.  The large input files were under 1 second.
Your times may vary due to hardware, so don't use these
as a strict guide.

