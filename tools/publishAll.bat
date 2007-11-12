@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
perl -S %0.bat
goto endofperl
';

open(FILES, "baseVMfiles |");
while (<FILES>) {
  chop;
  printf STDERR ".";
  `basePublish $_`;
}

__END__
:endofperl
