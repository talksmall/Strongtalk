@rem = 'Copyright 1996, Animorphic Systems. $Revision: 1.1 $
@rem extract interp/compiled/compiler times from profile output
@echo off
perl -S %0.bat %1
goto endofperl
';

local($file_name) = $ARGV[0];

sub getStats {
  open(FILE,$file_name) || die "could not open file: $filename";
  local($int) = 0;
  local($compiled) = 0;
  local($compiler) = 0;
  local($total) = 0;
  local($rest) = 0;
  local($first) = 1;
  while(<FILE>) {
    chop;
    if (/^Flatprofile (\d*(\.\d*)?) secs \((\d*) ticks\)/) {
      if (! $first) {
        $rest = $total - $int - $compiled - $compiler;
        printf "%d\t%d\t%d\t%d\n", $rest, $int, $compiled, $compiler;
      }
      $first = 0;
      $total = $3;
      $int = $compiler = $compiled = 0;
    }
    if (/(\d*(\.\d*)?)\% *(\d*) Total interpreted/) {
      $int = $3;
    }
    if (/(\d*(\.\d*)?)\% *(\d*) Total compiled/) {
      $compiled = $3;
    }
    if (/(\d*(\.\d*)?)\% *(\d*) Total compilation/) {
      $compiler = $3;
      #exit(0);
    }
  }
  close(FILE);
  if (! $first) {
    printf "%d\t%d\t%d\t%d\n", $total, $int, $compiled, $compiler;
  }
}


&getStats();

__END__
:endofperl
