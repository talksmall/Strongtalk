@rem = ' Copyright 1994, Animorphic Systems. $Revision: 1.4 $
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
$rcsfile   = "$Baseline/$sub/RCS/$file,v";

sub checkIn {
    local($dir, $file) = @_;
    local($rev_number) = 1;
    local($rev_name)   = "Genesis";
    
    chdir($dir);
    if (-f "RCS/$file,v") {
	`rcs -U -q $file`;
    }
    `ci -u$rev_number -m$rev_number -N$rev_name -q -t- $file`;
}

sub needDirectory {
    local($dir) = @_;
    if (! -d $dir) {
	mkdir($dir,0777) || die "Couldn't create $dir!\n";
    }
}

if (! -f $localfile) {
    die "Local file: $localfile does not exist!\n";
}

& needDirectory("$Baseline/$sub");
& needDirectory("$Baseline/$sub/RCS");

sub doit {
    printf STDERR "\nPublish: %s in %s.\n", $file, $sub;
    chmod(0666,$basefile);
    `cp $localfile $localfile.publish`;
    `cp $localfile $basefile`;
    &checkIn("$Baseline/$sub", $file);
    chmod(0444,$baseline);
    `cp $basefile $localfile`;
    chmod(0666,$localfile);
}


if (! -f $basefile) {
    &doit;
    exit(0);
}

chop($result =  `diff $basefile $localfile`);
exit(0) if $result eq "";

sub reverse{
  local($name) = @_;
  $name =~ s/\//\\/g;
  return $name;
}

$r_basefile  = &reverse($basefile);
$r_localfile = &reverse($localfile);

$base_rev = `getRevision $r_basefile`;
$local_rev = `getRevision $r_localfile`;

if ($base_rev eq $local_rev) {
  &doit;
} else {
  printf STDERR "Revision number in the local file conflicts\n";
  printf STDERR "with the revision number in the baseline file\n";
  printf STDERR "Local:    $localfile ($local_rev)\n";
  printf STDERR "Baseline: $basefile  ($base_rev)\n";
}
__END__
:endofperl
