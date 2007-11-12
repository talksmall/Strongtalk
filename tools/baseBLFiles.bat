@rem = ' Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
perl -S %0.bat
goto endofperl
';
sub dirname {
  local($name) = @_;
  $name =~ s/\\/\//g;
  $name =~ tr/A-Z/a-z/;
  return $name;
}

$Baseline = do dirname($ENV{'DeltaBaselineDir'});
$Delta    = do dirname($ENV{'DeltaDir'});

sub catdir {
  local($a,$b) = @_;
  return $a if $b eq "";
  return $b if $a eq "";
  return "$a/$b";
}

sub files {
  local($sub) = @_;
  local($dir) = &catdir($Baseline,$sub);  
  opendir(DIR,"$dir") || (warn "Can't open $dir: $!\n", return);
  local(@filenames) = readdir(DIR);
  closedir(DIR);
  for (@filenames) {
    next if $_ eq '.';
    next if $_ eq '..';
    next if $_ eq 'RCS';
    if (-d "$dir/$_") {
      do files(&catdir($sub,$_));
    } else {
      do wanted($dir, $_);
    }
  }
}

sub wanted {
  local($dir,$file) = @_;
  printf "%s %s\n", $sub, $file;
}

sub check_dir {
  local($sub) = @_;
  local($dir) = &catdir($Delta,$sub);  
  if (-d $dir) {
    do files($sub);
  }
}

do check_dir("bin");
do check_dir("vm");
do check_dir("world");
__END__
:endofperl
