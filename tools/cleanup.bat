@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.3 $
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

$Delta = do dirname($ENV{'DeltaDir'});

sub files {
  local($dir) = @_;
  opendir(DIR,"$dir") || (warn "Can't open $dir: $!\n", return);
  local(@filenames) = readdir(DIR);
  closedir(DIR);
  for (@filenames) {
    next if $_ eq '.';
    next if $_ eq '..';
    if (-d "$dir/$_") {
      do files("$dir/$_");
    } else {
      do wanted("$dir/$_");
    }
  }
}
1 ;

sub mydelete {
  local($file) = @_;
  printf "Deleting %s\n", $file;
  unlink $file;
}

sub wanted {
  local($file) = @_;
  do mydelete($file) if $file =~ /\.bak$|\.OLD$|\.publish$|\.~$/;
}

do files("$Delta");
__END__
:endofperl
