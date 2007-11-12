@rem = ' Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
cd %1
perl -S %0.bat %1 %2
goto endofperl
';

require "delta.pl";

local($file) =$ARGV[1];

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
`windiff $ARGV[0]\\$ARGV[1] $Baseline\\$sub\\$ARGV[1]`;
__END__
:endofperl
