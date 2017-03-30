<html>

<head>
	<title>Message Login</title>
	<link href="style.css" type="text/css" rel="stylesheet">
</head>

<body>

<?php
exec("./create index.wpml", $output, $result);
include "interpret.php";

echo "<input type=\"hidden\" name=\"first\" value=\"yes\" form=\"gotoview\">";
?>

</body>
</html>
