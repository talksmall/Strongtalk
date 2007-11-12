@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.4 $
@echo off
perl -S %0.bat %1
goto endofperl
';

sub dirname {
  local($name) = @_;
  $name =~ s/\\/\//g;
  $name =~ tr/A-Z/a-z/;
  return $name;
}

chop($CWD = `cd`);
$CWD       = do dirname($CWD);
$Delta     = do dirname($ENV{'DeltaDir'});

local($file) =$ARGV[0];

sub deltaSubDir {
  if ($CWD eq $Delta) {
    return ".";
  }
  local($res) = rindex("$CWD","$Delta",0);
  if ($res == -1) {
    die "You must be in a sub directory of $Delta!";
  }
  return substr($CWD, 1 + length($Delta) - length($CWD));
}

$sub = &deltaSubDir;
`baseCompare $sub $file`
__END__
:endofperl
