#!/usr/bin/env perl
use strict;

my $FILENAME = $ARGV[0];
my $BAKFILE = $ARGV[0] . ".bak";

my $mode = 1;  # 0: tex->tex-with-speedups  1: tex->data for gnuplot

# sleep 0.5;
# system ("cp -p $FILENAME $BAKFILE");

# sleep 0.5;
# open (IN, "< $BAKFILE")
#     or die "can't open $BAKNAME\n";
# open (OUT, "> $FILENAME")
#     or die "can't open $FILENAME\n";

open (IN, "< $FILENAME");

my $base;
my $graph;

while (<IN>) {
  if ( $_ =~ /^\s*serial\s*&\s*(\S+)\s*&.*/ ) {
    if ($mode==0) { print stdout $_; }
    $base = $1;
  } elsif ( $_ =~ /^(\S+) \\\\ %graph \"\S+\"$/ ) {
    if ($mode==1) { print stdout "# "; }
    $graph = $2;
    print stdout $_;
  } elsif ( $_ =~ /Tascell\S+\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
            or
            $_ =~ /serial\\_cal\S+\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
          ) {
    if ($mode==1) { print stdout "# $graph"; }
    if ($mode==1) { print stdout "# "; }
    print stdout $_;
    my @arr = ($1,$2,$3,$4,$5,$6,$7,$8);
    my $i=0;
    foreach my $tm (@arr) {
      my $speedup;
      my $round;
      $i++;
      if ( $tm eq '---' ) {
        if ($mode==0) {
          print stdout "& ";
        }
      } else {
        $speedup = $base / $tm;
        if ( $speedup < 1 ) {
          $round = sprintf("%.3f",$speedup);
        } else {
          $round = sprintf("%.2f",$speedup);
        }
        if ($mode==0) {
          print stdout "& ($round) ";
        } elsif ($mode==1) {
          print stdout "$i $round\n";
        }
      }
    }
    if ($mode==0) { print stdout "\\\\  \\hline\n"; }
  } else {
    if ($mode==0) {print stdout $_;}
  }
}

close (IN);
# close (OUT);
