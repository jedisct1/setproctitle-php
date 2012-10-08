--TEST--
Check for setproctitle presence
--SKIPIF--
<?php if (!extension_loaded("setproctitle")) print "skip"; ?>
--FILE--
<?php 
echo "setproctitle extension is available";
?>
--EXPECT--
setproctitle extension is available
