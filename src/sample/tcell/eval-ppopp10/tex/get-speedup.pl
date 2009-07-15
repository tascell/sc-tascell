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

open (IN, "< $FILENAME")
    or die "Can't open $FILENAME: $!";

my $base;
my $graph;

while (<IN>) {
    if ( $_ =~ /^\s*serial\s*&\s*(\S+)\s*&.*/ ) {
        if ($mode==0) { print stdout $_; }
        $base = $1;
    } elsif ( $_ =~ /^(\S+)\s+\\\\\s+%graph\s+\"(\S+)\"\s+$/ ) {
        $graph = $2;
        if ($mode==1) { print stdout "# $2\n"; }
        print stdout $_;
    } elsif ( $_ =~ /(Tascell\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
              or
              $_ =~ /(Cilk\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
              or              
              $_ =~ /(serial\\_cal\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
        ) {
        my $impl = $1;
        $impl =~ tr/\\//d;
        my $outfile  = "$graph-$impl.dat";
        if ($mode==1) {
            open (OUT, "> $outfile")
                or die "can't open $FILENAME: $!";
        }
        if ($mode==1) { 
            print OUT "# $graph-$impl\n";
            print OUT "# $_";
        } elsif ($mode==0) {
            print stdout $_;
        }
        my @arr = ($2,$3,$4,$5,$6,$7,$8,$9);
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
                    print OUT "$i $round\n";
                }
            }
        }
        if ($mode==0) { print stdout "\\\\  \\hline\n"; }
        if ($mode==1) { close(OUT); }
    } else {
        if ($mode==0) {print stdout $_;}
    }
}

close (IN);
# close (OUT);
