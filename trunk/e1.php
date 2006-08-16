<?php
$a =<<<EOT
Hello {{NAME}},
   You have just won $ {{VALUE}}!
   {{#IN_CA}}Well, $ {{TAXED_VALUE}}, after taxes.{{/IN_CA}}

{{#ONE_RESULT}}
{{#SUBITEM_SECTION}}<blockquota>{{/SUBITEM_SECTION}}
{{I}}
{{JUMP_TO_URL:h}}
{{/ONE_RESULT}}
EOT;

	$tpl = new ctemplate ("test", $a, DO_NOT_STRIP);
	$dict = new ctemplate_dict ();
	$dict->set ("NAME", "John Smith");
	$winnings = rand() % 100000;
	$dict->set ("VALUE", $winnings, "IN_CA");
	$value = sprintf ("%.2f", $winnings * 0.83);
	$dict->set ("TAXED_VALUE", $value);
	//for ($i=0;$i<10;$i++)
	//{
		$one = $dict->addsection ("ONE_RESULT");
		//$one->set ("I", $i);
		$one->show ("SUBITEM_SECTION");
		$one->setescaped ("JUMP_TO_URL", "http://fatpipi.com/1.php?tes", 0);
	//}

	$out = $tpl->expand ($dict);
	echo $out;
?>
