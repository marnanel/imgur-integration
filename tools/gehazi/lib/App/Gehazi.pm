package App::Gehazi;

use 5.005;
use strict;
use warnings;

use App::Gehazi::Skin;

our $VERSION = 0.01;

sub handle {
	my %output;

	%output = (
		_form => 'html',
		content => 'This is Gehazi.',
	);

	$output{'_headers'} = 1;

	print App::Gehazi::Skin::handle(%output);
}

1;

=head1 NAME

App::Gehazi - simple image uploading to your own server

=head1 AUTHOR

Thomas Thurman <thomas@thurman.org.uk>

=head1 SYNOPSIS

  ...

=head1 DESCRIPTION

  ...

=head1 COPYRIGHT

This Perl module is copyright (C) Thomas Thurman, 2011.
This is free software, and can be used/modified under the terms of the GNU GPL version 2.

