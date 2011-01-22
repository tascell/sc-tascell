#!/usr/bin/perl
my $STARTHOST=$ARGV[0];
my $N=$ARGV[1];
my $LEN;

my $PREFIX;
my $START;
if ( $STARTHOST =~ /([^0-9 \t\n\r\f]+)([0-9]+)/ ) {
    $PREFIX = $1;
    $START = $2;
    $LEN = length $2;
} else {
    print "Invalid STARTHOST: $STARTHOST\n";
    exit 1;
}

my $STR='';

foreach my $i ($START..($START+$N-1)) {
    $STR .= $PREFIX;
    $STR .= substr (sprintf ("%0100d",$i), 100-$LEN);
    if ($i<($START+$N-1)) {
        $STR .= '|';
    }
}

print "$STR\n";
