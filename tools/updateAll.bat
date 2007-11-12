@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
perl -S %0.bat
goto endofperl
';

open(FILES, "baseBLFiles |");
while (<FILES>) { 
  chop;
  printf STDERR ".";
  `baseUpdate $_`; 
}

__END__
:endofperl
