@rem = ' Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
perl -S %0.bat %1 %2
goto endofperl
';

local($sub)  = $ARGV[0];
local($file) = $ARGV[1];

sub dirname {
    local($name) = @_;
    $name =~ s/\\/\//g;
    $name =~ tr/A-Z/a-z/;
    return $name;
}

$Baseline  = do dirname($ENV{'DeltaBaselineDir'});
$Delta     = do dirname($ENV{'DeltaDir'});

$localfile = "$Delta/$sub/$file";
$basefile  = "$Baseline/$sub/$file";

die "Local file: $localfile does not exist!\n" if ! -f $localfile;
die "Base file:  $basefile does not exist!\n"  if ! -f $basefile;
chop($result =  `cmp $basefile $localfile`);
if ($result ne "") {
  printf STDERR "\n[%s/%s]\n", $sub, $file;
  open(LINES, "diff $basefile $localfile |");
  while (<LINES>) { 
    chop;
    printf STDERR "  %s\n", $_;
  }
}
__END__
:endofperl
