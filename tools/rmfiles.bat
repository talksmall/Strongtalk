@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
perl -S %0.bat
goto endofperl
';

while (<STDIN>) { 
  chop;
  printf STDERR "  deleting $_\n";
  unlink $_;
}

__END__
:endofperl
