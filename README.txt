Caveats
-------
The Strongtalk project only started up again recently after having been inactive since 1996, and all VM development stopped at that point.   It is now starting up again, since Strongtalk is still by far the fastest Smalltalk implementation in existence, and is fully open-source unlike any other fast Smalltalk implementations, so there is much of value here.

While the C++ parts of the system now build under VisualStudio 2005 (and the Express version), the assembler files still require Borland Turbo Assembler.  To enable compilation without purchasing the assembler, we have included the .obj files in COFF format from the assembler, for convenience (in bin/asm_objs.

Requirements
------------
In order to run the Strongtalk VM, the following is required:

1) Intel Pentium PC, 90MHz, at least 32MB of RAM.

2) Windows NT 4.0 or Windows 95 or higher.

In order to build the Strongtalk VM from scratch, additionally the following
software is required:

3) MS VisualStudio 2005 or VisualStudio 2005 Express

2) Assembler .obj files are included in the packaged source release, but if 
   you wish to reassemble them,
   you will need Borland Turbo Assembler version 4.0. 
   We don't know if things work properly with a newer version.
   An installed version of the assembler should be placed in
   the tools\tasm directory - extra installation may only be required
   if there's a problem with the current setup.

Build instructions
------------------

We will call the root strongtalk directory $STRTLK.

1) Build the 'makedeps' utility:  In Explorer, navigate 
   to $STRTLK\tools\makedeps. Open the VisualStudio project file for 
   makedeps, makedeps.vcproj.  "Run Build/Build Solution" from the menu.  This
   should build $STRTLK\tools\makedeps.exe

2) Use makedeps to build include file dependencies: From a command line, 
   cd into the $STRTLK\bin directory.  Run 'nmake -f Makefile.win32 lists'.

3) From Windows Explorer, start the VisualStudio project for Strongtalk by 
   opening $(STRTLK)\bin\strongtalk.vcproj.

4) In the VisualStudio IDE, choose Build -> Batch Build.
   Select the project configurations you want to build from the choices of
   Debug, Fast, and Product (ignore Release, that is spurious right now)
   and press Rebuild All.

5) Remove intermediate files if desired from the command file in $STRTLK\bin:
   nmake -f Makefile.win32 clean

The executables end up in the $STRTLK directory.  Intermediate build files (object files etc) end up in subdirectories of $STRTLK\bin.

There are three configurations in the project - debug, fast and product.
Each has its own subdirectory under bin, and its own executable: 
strongtalk_debug.exe, strongtalk_fast.exe and strongtalk.exe, respectively.
The differences between them are: 

- The debug configuration turns off C++ optimizations, turns on debugging 
  options, and also turns on extensive assertions in the code, so it runs 
  slowly.

- The fast configuration is like the debug configuration except that it turns 
  off assertions and turns on more C++ optimizations, so it runs quite fast 
  yet is still debuggable.  This is a good one to use if you want to get real
  work done in it but still want to be able to debug crashes when they happen.

- The product configuration turns off all the debugging options, links with
  the non-debug runtime libraries, and turns on more extensive C++ 
  optimizations.  This is the version that should be used for benchmarking.

Execute instructions
--------------------

1) In the $STRLK directory, just run the VM executable you wish to use;
  for the product version that would be:

	strongtalk

  Files needed in that directory for running an application are: 
  
     strongtalk.bst - the image file

     .strongtalkrc - the VM configuration options file

  If you wish to use the development environment, you will also need:

     source - folder containing the source code for the Strongtalk libraries




Special situations
__________________

If you do have access to TASM, you should be able to get by by using the object
files that are generated from the assembly code. If you are using  MS VC++ 4.0,
you should probably be able do place the object files found in strongtalk\bin\asm_objs\OMF
in the build subdirectories (debug, fast, product) and doing an incremental build. 

If you don't have an old version of MS VC++, you will either have to wait until
we provide a version of the code that builds under a newer C++ compiler, or try
and convert the code yourself. 

Issues: if you get the C++ code to compile, you
will find that VisualStudio 2005 will not link with the object files produced by TASM, which are in OMF format.  
You can substitute these with the COFF versions
found in strongtalk\bin\asm_objs\COFF and repeat the link phase. This should work
(it worked for us). 

If you want to build on a non-windows machine, we're all for it, but you have non-trivial work to do.

Problems we have encountered
----------------------------
Occasionally, despite strict adherence to the build instructions, we have run into problems on some machines, 
probably due to pre-existing installations of later versions of tools, environment variable differences, etc.
- Link errors (LNK1104): It appears that on some machines, some libraries (e.g. kernel32.lib) cannot be found. 
By setting the full path (e.g., "C:\Program Files\MSDEV\LIB\kernel32.lib") we were able to circumvent this problem.
You can do this in MSDEV by choosing Build>Settings and choosing the Link tab, and ensuring that every library that is missing is
listed explicitly with its full path name in the Object files list. All 3 projects should have the same list of libraries:
"c:\Program Files\msdev\lib\kernel32.lib" "c:\Program Files\msdev\lib\user32.lib" "c:\Program Files\msdev\lib\gdi32.lib" "c:\Program Files\msdev\lib\winspool.lib" "c:\Program Files\msdev\lib\comdlg32.lib" "c:\Program Files\msdev\lib\advapi32.lib" "c:\Program Files\msdev\lib\shell32.lib" "c:\Program Files\msdev\lib\ole32.lib" "c:\Program Files\msdev\lib\oleaut32.lib" "c:\Program Files\msdev\lib\uuid.lib" "c:\Program Files\msdev\lib\LIBCD.lib" "c:\Program Files\msdev\lib\oldnames.lib" 

You can paste this in the Object files field of the Links tab under Buikd>Settings in MSDEV.
