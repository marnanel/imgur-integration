#!/usr/bin/perl
use strict;
use warnings;

my $plugin = $ARGV[0];

die "Please give the name of a plugin to enable.\n" unless $plugin;

my $current = qx(gconftool-2 -g /apps/eog/plugins/active_plugins);

chomp $current;
$current =~ s/^\[//;
$current =~ s/\]$//;

my @current = split /,/, $current;

die "$plugin is already enabled.\n" if grep { $_ eq $plugin } @current;

my $fixed = join(',', @current, $plugin);

print qx(gconftool-2 -s /apps/eog/plugins/active_plugins --type list --list-type string "[$fixed]");
print "I have enabled $plugin.\n";
