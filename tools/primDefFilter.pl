#!/usr/bin/perl
# 'Copyright 1994, Animorphic Systems. $Revision: 1.3 $

sub dirname {
  local($name) = @_;
  $name =~ s/\\/\//g;
  $name =~ tr/A-Z/a-z/;
  return $name;
}

$Delta = do dirname($ENV{'DeltaDir'});

$Directory = "$Delta/vm/prims";

opendir(DIR,$Directory) || die "Can't open vm/prims!\n";
@filenames = readdir(DIR);
closedir(DIR);

sub generate {
  local($filename) = @_;
  local($inside)   = 0;
  open(FILE,"$Directory/$filename") || die "Can't open $filename";
  for (<FILE>) {
    chop;   # remove the newline
    if ($inside) {
       if (/^[ \t]*\/\/%/) {
         printf "!\n";
         $inside = 0;
       } else {
         if (/^[ \t]*\/\/(.*)/) {
           printf "%s\n", $1;
         } else {
           die "Wrong format inside primitive definition";
         }
       }
    } else {
       if (/^[ \t]*\/\/%prim/) {
         $inside = 1;
       }
    } 
  }
  if ($inside) {
    die "Primitive definition not completed";
  }
  close(FILE)
}

for (@filenames) {
  do generate($_) if $_ =~ /[a-zA-Z]+_prims.hpp$/
}
printf "!\n"
