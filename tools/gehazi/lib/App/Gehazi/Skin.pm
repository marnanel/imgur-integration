package App::Gehazi::Skin;

use strict;
use warnings;

sub handle {
	my (%details) = @_;

	die "Need a 'form' parameter.\n" unless defined $details{'form'};

	my $form = $details{'form'};

	my $result = '';

	if ($form eq 'rsp' || $form eq 'err') {
		$result .= "<$form>\n";
		$result .= "</$form>\n";
	} else {
		die "Unknown form: $form\n";
	}

	return $result;
}

1;

