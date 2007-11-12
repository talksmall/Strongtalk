
set src=%DeltaDir%
set drive=v:
set dst=%drive%\%1
mkdir %dst%
mkdir %dst%\vm

mkdir %dst%\vm\asm
copy %src%\vm\asm          %dst%\vm\asm

mkdir %dst%\vm\code
copy %src%\vm\code         %dst%\vm\code

mkdir %dst%\vm\compiler
copy %src%\vm\compiler     %dst%\vm\compiler

mkdir %dst%\vm\disasm
copy %src%\vm\disasm       %dst%\vm\disasm

mkdir %dst%\vm\incls
copy %src%\vm\incls        %dst%\vm\incls

mkdir %dst%\vm\interpreter
copy %src%\vm\interpreter  %dst%\vm\interpreter

mkdir %dst%\vm\lookup
copy %src%\vm\lookup       %dst%\vm\lookup

mkdir %dst%\vm\memory
copy %src%\vm\memory       %dst%\vm\memory

mkdir %dst%\vm\oops
copy %src%\vm\oops         %dst%\vm\oops

mkdir %dst%\vm\prims
copy %src%\vm\prims        %dst%\vm\prims

mkdir %dst%\vm\recompiler
copy %src%\vm\recompiler   %dst%\vm\recompiler

mkdir %dst%\vm\runtime
copy %src%\vm\runtime      %dst%\vm\runtime

mkdir %dst%\vm\topIncludes
copy %src%\vm\topIncludes  %dst%\vm\topIncludes

mkdir %dst%\vm\utilities
copy %src%\vm\utilities    %dst%\vm\utilities

mkdir %dst%\vm\bin
sed -e "s/USERS/users/g" < %src%\vm\bin\Delta.mak | sed -e "s/users.larsp.Delta/%1/g" > %dst%\vm\bin\Delta.mak
copy %src%\vm\bin\Delta.mdp  %dst%\vm\bin

mkdir %dst%\vm\bin\asm_obj
copy %src%\vm\bin\asm_obj\Makefile  %dst%\vm\bin\asm_obj

%drive%
cd %dst%\vm\bin\asm_obj
make

cd %dst%\vm\bin
compileO
