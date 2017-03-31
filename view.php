<html>

<head>
	<title>Viewing Streams</title>
	<link href="style.css" type="text/css" rel="stylesheet">
</head>

<body>

<?php

/* recalculate $length every time. */
$length = shell_exec("./view.py \"".$_POST["username"]."\" ".$_POST["stream"]." 0 pdate len");

$index = '0';
$order= 'pdate';
$command = 'view';

/* this would occur if we just got into the page. */
if(!isset($_POST["index"]))
{
	if($_POST["stream"] === "all")
	{
		$index = 0;
	}
	else
	{
		$eres = shell_exec("./view.py \"".$_POST["username"]."\" ".$_POST["stream"]." 0 pdate last");

		$index = $eres;
	}
}
else
{
	$index = $_POST["index"];
}


if(!isset($_POST["order"]))
{
	$order = "pdate";
}
else
{
	$order = $_POST["order"];
}

/* this reacts to the user inputs. */
if(isset($_POST["userin"]))
{
	$command = $_POST["userin"];

	if($command == "next")
	{
		$index = intval($_POST["index"]) + 1;
		if($index >= $length)
		{
			$index = $length - 1;
		}
	}
	elseif($command == "prev")
	{
		$index = intval($_POST["index"]) - 1;
		if($index < 0)
		{
			$index = 0;
		}
	}
	else
	{
		$index = $_POST["index"];
	}

	if($command == "mark")
	{
		$res = shell_exec("./view.py \"".$_POST["username"]."\" \"".$_POST["stream"]."\" \"".$_POST["order"]."\" mark");
	}

	elseif($command == "order")
	{
		if($order === 'pdate')
		{
			$order = 'username';
		}
		elseif($order === 'username')
		{
			$order = 'pdate';
		}
		$index = 0;
	}
}

$username = $_POST["username"];
$stream = $_POST["stream"];

$cmd = "./view.py '$username' '$stream' '$index' '$order' 'view'";
$post = shell_exec($cmd);#"./view.py julian bunnies 0 date view");

str_replace("\n", "<br>", $post);
$post = nl2br($post);

exec("./create view.wpml", $output, $result);
include "interpret.php";

echo "<input type=\"hidden\" form=\"command\" name=\"index\" value=\"$index\">";
echo "<input type=\"hidden\" form=\"switch\" name=\"index\" value=\"$index\">";

echo "<input type=\"hidden\" form=\"command\" name=\"order\" value=\"$order\">";
echo "<input type=\"hidden\" form=\"switch\" name=\"order\" value=\"$order\">";

?>

</body>

</html>
