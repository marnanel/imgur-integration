package App::Gehazi::Config;

use strict;
use warnings;

my %fields;

sub set {
	my ($field, $value) = @_;

	$fields{$field} = $value;
}

sub get {
	my ($field, $default) = @_;

	return $fields{$field} if defined $fields{$field};
	return $default;
}

1;

