<html>
<body>

<head>
	<title>Post Submitted</title>
	<link href="style.css" type="text/css" rel="stylesheet">
</head>

<?php

exec("./create post.wpml", $output, $result);
include "interpret.php";

if(isset($_POST["postpressed"]))
{
	exec("./post \"".$_POST["username"]."\" ".$_POST["stream"]." \"".$_POST["post"]."\n\"", $pout, $pres);

	echo "<div id=\"card\">";
	if($pres > 0)
	{
		echo "<br><br><p style=\"text-align: center\">Something went wrong while postng.<br>Nothing seems to have been written.</p><br><br>";
	}
	else
	{
		echo "<br><br><p style=\"text-align: center\">Post Submitted!</p><br><br>";
	}
	echo "</div>";
}

?>
</body>
</html>
