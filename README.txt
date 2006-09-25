Caveats
-------
The Strongtalk project has not been active since 1996, and all VM
development stopped at that point. We have tried to reconstruct the
build process as much as possible, but currently it still relies
on somewhat dated tools. 

We have been able to compile and link a version using MS Visual Studio 2005 after updating 
the sources to conform to the current C++ standard. However,
this version does not yet run (immediate crash at startup). To avoid delaying 
this release any further, we decided to make the original sources and build
process available as is.

We realize this is inconvenient, and you may find it difficult to get hold of these tools. We apologize, 
and hope to sort out these issues in the near future.

Requirements
------------
In order to run the Strongtalk VM, the following is required:

1) Intel Pentium PC, 90MHz, at least 32MB of RAM.

2) Windows NT 4.0 or higher.
In order to build the Strongtalk VM from scratch, additionally the following
software is required:

1) MS VC++ 4.0
   Note that currently this workspace may not build with higher versions!
   It certainly does not build with MS VisualStudio 2005, due to changes in the C++ standard.
   Proper setup of the path environment variable is required, your path should include
   C:\Program Files\MSDEV\bin

2) Borland Turbo Assembler version 4.0. 
   We don't know if things work properly with a newer version.
   An installed version of the assembler should be placed in
   the tools\tasm directory - extra installation may only be required
   if there's a problem with the current setup.

3) Unix tools or MKS toolkit. An installed version of the MKS toolkit
   should be placed in tools\mks. Proper setup of the path environment
   variable is required: assuming the strongtalk directory is at the top level of drive X,
   your path should include X:\strongtalk\tools\mks\mksnt.
   Make sure that the environment variable ROOTDIR is defined as X:\strongtalk\tools\mks,
   where X is the disk on which you have installed strongtalk.


Installation instructions
-------------------------

1) Copy the entire strongtalk directory to the desired destination.
  The system has been tested with the strongtalk directory at the top level on the target drive; e.g. C:\strongtalk.
  If you install it elsewhere, you will have to adjust the path settings given above.


Build instructions
------------------

1) cd into the bin directory of the copied strongtalk directory. Create include
   dependencies by typing:
   make lists

2) Start the MS VC++ 4.0 IDE:
   msdev strongtalk.mdp

3) In the MS VC++ 4.0 IDE, choose Build -> Batch Build... . Select all
   project configurations you want to build and press Rebuild All.

4) Remove intermediate files if desired:
   make clean

The executables end up in the strongtalk directory. Intermediate build products (object files)
end up in subdirectories of strongtalk\bin


There are three projects in the workspace - debug, fast and product.
Each has its own subdirectory under bin, and its own executable: 
strongtalk_debug.exe, strongtalk_fast.exe and strongtalk.exe.
The debug project is (surprise) for debugging. The product version is what you
would want to use for benchmarking, etc.


Execute instructions
--------------------

1) In the strongtalk directory, type:
   strongtalk


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
