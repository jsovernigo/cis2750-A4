<html>

<head>
	<title>Add or Remove from Streams</title>
	<link href="style.css" type="text/css" rel="stylesheet">
</head>

<body>

<?php

/* this is used to add/remove something to a stream. */
if(isset($_POST["action"]))
{
	if($_POST["action"] == "add")
	{
		$sstring = "";
		foreach($_POST["streams"] as $stream)
		{
			$sstring = $sstring.$stream.',';
		}
		$sstring = $sstring.$_POST["newstreams"];
		$username = $_POST["username"];
		if($sstring{strlen($sstring) - 1} === ',')
		{
			$sstring = substr($sstring, 0, -1);
		}
		exec("./addauthor $username $sstring", $aout, $ares);
	}
	elseif($_POST["action"] == "remove")
	{
		$sstring = "";
		foreach($_POST["streams"] as $stream)
		{
			$sstring = $sstring.$stream.',';
		}
		$sstring = substr($sstring, 0, -1);
		$username = $_POST["username"];

		exec("./addauthor $username $sstring -r", $aout, $ares);
	}
}

exec("./create addauthor.wpml", $output, $result);
include "interpret.php";

?>

</body>
</html>
