@rem = 'Copyright 1994, Animorphic Systems. $Revision: 1.2 $
@echo off
perl -S %0.bat %1
goto endofperl
';

local($file_name) = $ARGV[0];

sub getStatFor {
  local($name) = @_;
  open(FILE,$file_name) || die "could not open file: $filename";
  printf "%s\t", $name;
  local($mm) = 9999999;
  while(<FILE>) {
    chop;
    if (/^$name\t([0-9]*)/) {
      if ($mm > $1) {
        $mm = $1;
      } 
    }
  }
  printf "%d", $mm;
  printf "\n";

  close(FILE);
  return "";
}

sub getSmopFor {
  local($name) = @_;
  open(FILE,$file_name) || die "could not open file: $filename";
  printf "%s\t", $name;
  local($mm) = 999999;
  local($nn) = 0;
  while(<FILE>) {
    chop;
    if (/^(\d*(\.\d*)?)     (\d*(\.\d*)?)     $name/) {
      if ($mm > $1) {
        $mm = $1;
        $nn = $3;
      } 
    }
  }
  printf "%s\t%s", $mm, $nn;
  printf "\n";

  close(FILE);
  return "";
}

sub getSlopFor {
  local($name) = @_;
  open(FILE,$file_name) || die "could not open file: $filename";
  printf "%s\t", $name;
  local($mm) = 9999999;
  local($nn) = 0;
  while(<FILE>) {
    chop;
    if (/^\d*     (\d*(\.\d*)?)     \d*     (\d*(\.\d*)?)     $name/) {
      if ($mm > $1) {
        $mm = $1;
        $nn = $3;
      } 
    }
  }
  printf "%s\t%s", $mm, $nn;
  printf "\n";

  close(FILE);
  return "";
}

&getStatFor("BubbleSort");
&getStatFor("BubbleSort2");
&getStatFor("IntMM");
&getStatFor("IntMM2");
&getStatFor("MM");
&getStatFor("MM2");
&getStatFor("Perm");
&getStatFor("Perm2");
&getStatFor("Queens");
&getStatFor("Queens2");
&getStatFor("Quicksort");
&getStatFor("Quicksort2");
&getStatFor("Towers");
&getStatFor("Towers2");
&getStatFor("TreeSort");
&getStatFor("TreeSort2");
&getStatFor("Puzzle");
&getStatFor("Sieve");
&getStatFor("SumTo");
&getStatFor("Recurse");
&getStatFor("AtAllPut");
&getStatFor("IncrementAll");
&getStatFor("NestedLoop");
&getStatFor("Tak");
&getStatFor("Takl");
&getStatFor("Dictionary");
&getStatFor("OrderedCollection");
&getStatFor("Livermore");

printf "\n";
&getStatFor("Richards");

printf "\n";
&getStatFor("DeltaBlue");

printf "\n";
&getSlopFor("add integers");
&getSlopFor("add floats");
&getSlopFor("access strings");
&getSlopFor("create objects");
&getSlopFor("copy objects");
&getSlopFor("perform selectors");
&getSlopFor("evaluate blocks");

printf "\n";
&getSmopFor("generating fractonaccis");
&getSmopFor("generating primes");
&getSmopFor("generating and parsing streams");
&getSmopFor("generating strings");
&getSmopFor("forming sets");
&getSmopFor("sorting strings");
&getSmopFor("sorcerer's apprentice");

__END__
:endofperl
