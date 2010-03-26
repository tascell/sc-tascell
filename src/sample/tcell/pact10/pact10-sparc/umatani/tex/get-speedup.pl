#!/usr/bin/env perl
# 1,2,3,4,5,6,7,8,16,32,64,128
use strict;

my $FILENAME = $ARGV[0];
my $BAKFILE = $ARGV[0] . ".bak";

my $mode = $ARGV[1];  # 0: tex->tex-with-speedups  1: tex->data for gnuplot
print stderr "mode: $mode\n";

if ($mode == 1) {
    system ("rm -f *-bar.dat");
}

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
my $graphcount=0;

while (<IN>) {
    if ( $_ =~ /^(\S+)\s+\\\\\s+%graph\s+\"(\S+)\"\s+$/ ) {
        $graph = $2;
        $graphcount++;
        if ($mode==1) { print stdout "# $2\n"; }
        print stdout $_;
    } elsif ( $_ =~ /^\s*serial\s*&\s*(\S+)\s*&.*/ ) {
        if ($mode==0) { print stdout $_; }
        if ($mode==1) {
            # 棒グラフ用
            my $impl = "serial";
            my $core = 1;
            my $round = 1;
            my $barfile = "$impl-bar.dat";
            open (BAROUT, ">> $barfile")
                or die "Can't open $barfile: $!";
            print BAROUT "# $impl $graph $core core\n";
            print BAROUT "$graphcount $round\n";
            close (BAROUT);
        }
        $base = $1;
    } elsif ( $_ =~ /^(Tascell\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
              or
              $_ =~ /^(Cilk\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
              or              
              $_ =~ /^(serial\\_cal\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*&\s*(\S+)\s*\\\\.*/
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
            my $line = $_;
            $line =~ s/\\hline//;
            print stdout $line;
        }
        my @arr = ($2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13);
        my @core_arr = (1,2,3,4,5,6,7,8,16,32,64,128);
        my $core_id=0;
        
        foreach my $tm (@arr) {
            my $speedup;
            my $round;
            $core_id++;
            my $core = $core_arr[$core_id];
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
                    print OUT "$core $round\n";
                    if ( $core == 1 ) {
                        # 棒グラフ用
                        my $barfile = "$impl-bar.dat";
                        open (BAROUT, ">> $barfile")
                            or die "Can't open $barfile: $!";
                        print BAROUT "# $impl $graph $core core\n";
                        print BAROUT "$graphcount $round\n";
                        close (BAROUT);
                    }
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
