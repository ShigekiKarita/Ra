#!/usr/bin/env perl

use strict;
use warnings;
use 5.10.0;
use utf8;


my $filename = 'generated_preprocessors';
my $path = $filename . ".hpp";
open my $fh, '>', $path or die qq/Can't open file "$path" : $!/;

my $max = 256;


sub gen_pp {
    my ($name, $proc, $is3args, $min) = @_;    

    if (!$proc) {
        print $fh "#define ${name}_0(m,p)\n";
    }

    if (!defined($min)) {
        $min = 1;
    }
    
    for (my $i = $min; $i <= $max; ++$i)
    {
        my $lhs = "#define ${name}_${i}(m,p)";
        my $j   = $i - 1;        
        my $mhs = $is3args ? " ${name}_${j}(m,p) " : " ";
        my $rhs = $proc ? $proc->($i) : "m(${i},p)";

        print $fh "${lhs}${mhs}${rhs}\n";
    }
}



# ifdef guard
my $gname = uc($filename) . "_H";

print $fh "#ifndef ${gname}
#define ${gname} 1

";

# pp_repeat
gen_pp("pp_repeat", '', 1);
gen_pp("pp_last_repeat");
print $fh "
#define pp_repeat(n, m, l, p) pp_cat(pp_repeat_, pp_dec(n))(m,p) pp_cat(pp_last_repeat_,n)(l,p)
";

# pp_inc
print $fh "
#define pp_inc( n ) pp_inc_n( n )
#define pp_inc_n( n ) pp_cat( pp_inc_, n )
";
gen_pp("pp_inc", sub {$_[0] + 1}, '', 0);

# pp_dec
print $fh "
#define pp_dec( n ) pp_dec_n( n )
#define pp_dec_n( n ) pp_cat( pp_dec_, n )
";
gen_pp("pp_dec", sub {$_[0] - 1});

# endif guard
print $fh "
#endif // ${gname}
";

close $fh or die qw/Can't close file "$path": $!/;
