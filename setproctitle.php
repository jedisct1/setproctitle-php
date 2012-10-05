<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('setproctitle')) {
	dl('setproctitle.' . PHP_SHLIB_SUFFIX);
}
$module = 'setproctitle';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'setproctitle';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";
?>
