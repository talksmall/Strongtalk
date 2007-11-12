@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.3 $
@echo off
perl -S %0.bat %1
goto endofperl
';

local($name) = $ARGV[0];
sub getRevision {
  local($filename) = @_;
  open(FILE,$filename) || die "could not open file: $filename";
  while(<FILE>) {
    if (/\Revision:\s*(\S+)\s+/) {
      close(FILE);
      return $1;
    }
  }
  close(FILE);
  return "";
}
printf "%s", &getRevision($name);
__END__
:endofperl
