@rem = ' Copyright 1994, Animorphic Systems. $Revision: 1.5 $
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
$TMP       = do dirname($ENV{'TEMP'});

$localfile  = "$Delta/$sub/$file";
$basefile   = "$Baseline/$sub/$file";
$rcsfile    = "$Baseline/$sub/RCS/$file,v";
$tempfile   = "$TMP/merge.1";
$resultfile = "$TMP/merge.2";

sub cleanup {
  unlink($tempfile)   if -f $tempfile;
  unlink($resultfile) if -f $resultfile;  
  exit(0);
};

if (! -f $localfile) {
  printf STDERR "\nCreating %s", "$sub/$file";
  `cp $basefile $localfile`;
  `chmod +w $localfile`;
  exit 0;
}

die "Baseline file: $basefile does not exist!\n"  if ! -f $basefile;

sub reverse{
  local($name) = @_;
  $name =~ s/\//\\/g;
  return $name;
}

$r_basefile  = &reverse($basefile);
$r_localfile = &reverse($localfile);

$base_rev = `getRevision  $r_basefile`;
$local_rev = `getRevision $r_localfile`;

exit(0) if $base_rev == $local_rev;

($lmajor, $lminor) = split(/\./, $local_rev);
($bmajor, $bminor) = split(/\./, $base_rev);

if ($lmajor > $bmajor || $lmajor == $bmajor && $lminor > $bminor) {
  printf STDERR "Revision number in the local file is higher\n";
  printf STDERR "than the revision number in the baseline file\n";
  printf STDERR "Local:    $localfile ($local_rev)\n";
  printf STDERR "Baseline: $basefile  ($base_rev)\n";
  printf STDERR "Update aborted!\n";
  exit(1);
}

# $base_rev > $local_rev
# Now do the merge

printf STDERR "\nUpdating %s (%s -> %s)", "$sub/$file", $local_rev, $base_rev;


# check out the revision matching the local file
`co -q -p$local_rev $rcsfile > $tempfile`;

# do the merge
$merge_result = `merge -p $localfile $tempfile $basefile >  $resultfile`;

if ($merge_result == 2) {
  printf STDERR "Merge broke!\n";
  printf STDERR "Update aborted!\n";
  &cleanup;
}

if ($merge_result == 1) {
  printf STDERR "Merge conflicts in $sub/$file\n";
}

# make a backup of the old file
`mv $localfile $localfile.OLD`;

# save the new file 
`cp -f $resultfile $localfile`;

`chmod +w $localfile`;
&cleanup;
__END__
:endofperl
