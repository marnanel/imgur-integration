package App::Gehazi::Hash;

use strict;
use warnings;

sub random_char {
	my $n = int(rand(62));
	my $code;

	if ($n < 10) {
		# digits
		$code = $n + 48;
	} elsif ($n < 36) {
		# capitals
		$code = $n + 55;
	} else {
		# lowercase letters
		$code = $n + 61;
	}

	return chr($code);
}

sub hash {
	my ($length) = @_;

	$length = 6 unless defined $length;

	my $result = '';

	for (my $i=0; $i<$length; $i++) {
		$result .= random_char();
	}

	return $result;
}

1;
