<?php
	$tpl = new cTemplate ("example.tpl", STRIP_BLANK_LINES, null, TC_HTML);
	$dict = new cTemplate_Dict ();
	$dict->set ("NAME", "John Smith");

	$winnings = rand() % 100000;
	$dict->set ("VALUE", $winnings, "IN_CA");
	$value = sprintf ("%.2f", $winnings * 0.83);
	$dict->set ("TAXED_VALUE", $value);

    $out = $tpl->expand ($dict);
	echo $out;
?>
