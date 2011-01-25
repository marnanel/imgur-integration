package App::Gehazi::Skin;

use strict;
use warnings;

use File::Slurp;
use File::ShareDir qw(dist_file);

sub handle {
	my (%details) = @_;

	die "Need a '_form' parameter.\n" unless defined $details{'_form'};

	my $form = $details{'_form'};

	my $result = '';
	my $content_type = '';

	if ($form eq 'rsp' || $form eq 'err') {
		$result .= "<$form>\n";
		for my $field (sort keys %details) {
			next if $field =~ /^_/;
			$result .= "  <$field>$details{$field}</$field>\n";
		}
		$result .= "</$form>\n";
		$content_type = 'text/xml';
	} elsif ($form eq 'html') {
		$result = read_file(dist_file('App-Gehazi', 'template.html'));
		$content_type = 'text/html';

		my $title = 'Gehazi';
		$title = "$details{title} - Gehazi" if defined $details{'title'};

		$result =~ s/\%\(title\)s/$title/g;
		$result =~ s/\%\(content\)s/$details{content}/g;
	} else {
		die "Unknown form: $form\n";
	}

	if (defined $details{'_headers'}) {
		my $length = length($result);
		$result = "Content-Length: $length\nContent-Type: $content_type\n\n$result";
	}

	return $result;
}

1;

