@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.4 $
@echo off
perl -S %0.bat
goto endofperl
';

open(FILES, "baseVMfiles |");
while (<FILES>) {
  chop;
  ($dir, $file) = split(/ /,$_);
  $name = "$dir/$file";
  $localFiles{$name} = 1;
}

open(FILES, "baseBLfiles |");
while (<FILES>) {
  chop;
  ($dir, $file) = split(/ /,$_);
  $name = "$dir/$file";
  $baseFiles{$name} = 1;
}

for (sort(keys %localFiles)) {
  printf STDERR "%s (new locally)\n", $_ unless $baseFiles{$_};
}

for (sort(keys %baseFiles)) {
  printf STDERR "\t%s (new in baseline)\n", $_ unless $localFiles{$_};
}
__END__
:endofperl
