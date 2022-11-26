<?php

header('Content-Type: application/json');

define('DB_HOST', 'localhost');
define('DB_NAME', 'id19883322_adatok');
define('DB_USERNAME', 'id19883322_admin');
define('DB_PASSWORD', '*}Nv)[y6)G5dt[3Ij');

$mysqli = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);

if(!$mysqli){
  die("Connection failed: " . $mysqli->error);
}

$query = sprintf("SELECT TOP 10 TIME, HUM FROM arduino order by TIME desc");

$result = $mysqli->query($query);

$data = array();
foreach ($result as $row) {
  $data[] = $row;
}

$result->close();

$mysqli->close();

print json_encode($data);