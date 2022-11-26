<?php

header('Content-Type: application/json');

define('DB_HOST', 'localhost');
define('DB_NAME', 'id19883322_adatok');
define('DB_USERNAME', 'id19883322_admin');
define('DB_PASSWORD', 'ÁAdmin234567');


$mysqli = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);

if(!$mysqli){
  die("Connection failed: " . $mysqli->error);
}

$query = sprintf("SELECT TIME,TEMP FROM(SELECT TIME,TEMP FROM arduino order by TIME desc LIMIT 10)var1 ORDER BY TIME asc");

$result = $mysqli->query($query);

$data = array();
foreach ($result as $row) {
  $data[] = $row;
}

$result->close();

$mysqli->close();

print json_encode($data);