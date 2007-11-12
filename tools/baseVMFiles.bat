@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.5 $
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
  local($dir) = @_;
  opendir(DIR,"$Delta/$dir") || (warn "Can't open $dir: $!\n", return);
  local(@filenames) = readdir(DIR);
  closedir(DIR);
  for (@filenames) {
    next if $_ eq '.';
    next if $_ eq '..';
    if (-d "$Delta/$dir/$_") {
      do files(&catdir($dir,$_));
    } else {
      do wanted($dir, $_);
    }
  }
}

sub wanted {
  local($dir,$file) = @_;
  return if $file =~ /vmDate.cpp/;
  printf "%s %s\n", $dir, $file if   $file =~ /\.hpp$|\.cpp$|\.pl$|\.bat$/ 
                                  || $file =~ /\.asm$/
                                  || $file =~ /[mM]akefile$/
                                  || $file =~ /prims.src$|prims.inc$/
                                  || $file =~ /includeDB$|includeDB2$/
                                  || $file =~ /\.cls$|\.dlt$|\.gr$|\.str$/;
}

sub check_dir {
  local($sub) = @_;
  local($dir) = &catdir($Delta,$sub);  
  if (!-d $dir) {  return; }
  $dir = &catdir($Baseline,$sub);  
  if (!-d $dir) { return; }
  do files($sub);
}

do check_dir("bin");
do check_dir("vm");
do check_dir("world");
__END__
:endofperl
