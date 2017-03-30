<?php

$inphp = 0;
$script = "";
foreach($output as $line)
{
	if(substr($line, 0, 5) === "<?php" and substr($line, strlen($line) - 2, 2) === "?>")
	{
		eval(substr($line, 5, strlen($line) - 7));
		continue;
	}
	elseif(substr($line, 0, 5) === "<?php")
	{
		$inphp = 1;
		continue;
	}
	elseif(substr($line, strlen($line) - 2, 2) === "?>")
	{
		eval($script);
		$script = "";
		$inphp = 0;	
		continue;
	}

	if($inphp == 0)
	{
		echo $line;
	}
	else
	{
		$script = $script.$line;
	}
}

?>
