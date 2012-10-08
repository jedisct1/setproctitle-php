--TEST--
Check that setproctitle doesn't miserably crash
--SKIPIF--
<?php if (!extension_loaded("setproctitle")) print "skip"; ?>
--FILE--
<?php 
srand(0);
$i = 1000;
do {
  $title = str_repeat(chr(rand(1, 255)), rand(1, 255));
  setproctitle($title);
} while (--$i > 0);
echo 'ok';
?>
--EXPECT--
ok
