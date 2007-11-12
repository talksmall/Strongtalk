@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.1 $
@echo off
perl -S %0.bat %1
goto endofperl
';

local($remote)  = $ARGV[0];

opendir(DIR,".") || (warn "Can't open $dir: $!\n");
local(@filenames) = readdir(DIR);
closedir(DIR);
for (@filenames) {
  next if $_ eq '.';
  next if $_ eq '..';
  if (-f "$_") {
    $localFiles{$_} = 1;
  }
}

opendir(DIR,"$remote") || (warn "Can't open $dir: $!\n");
local(@filenames) = readdir(DIR);
closedir(DIR);
for (@filenames) {
  next if $_ eq '.';
  next if $_ eq '..';
  if (-f "$remote/$_") {
    $baseFiles{$_} = 1;
  }
}

for (sort(keys %localFiles)) {
  if ($baseFiles{$_}) {
    printf "\@diff -C 2 $remote/$_ $_\n";
  }
}

__END__
:endofperl
