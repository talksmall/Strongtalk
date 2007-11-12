@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
perl -S %0.bat %1
goto endofperl
';

open(FILES, "baseVMfiles |");
while (<FILES>) {
  chop;                     
  printf STDERR ".";
  `baseCompare $_`  if $_ !~ /prims.src$|prims.inc$/
}
__END__
:endofperl

