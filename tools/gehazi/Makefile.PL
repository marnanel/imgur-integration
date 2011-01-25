use inc::Module::Install;
  
name           'App-Gehazi';
all_from       'lib/App/Gehazi.pm';

requires       'File::ShareDir';
requires       'File::Slurp';
test_requires  'Test::More';

install_share;

WriteAll;
