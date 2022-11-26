<?php

header('Content-Type: application/json');

define('DB_HOST', 'localhost');
define('DB_USERNAME', 'id19883322_adatok');
define('DB_PASSWORD', 'id19883322_admin');
define('DB_NAME', '*}Nv)[y6)G5dt[3Ij');

$mysqli = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);

if(!$mysqli){
  die("Connection failed: " . $mysqli->error);
}

$query = sprintf("SELECT TIME, MOIST FROM arduino");

$result = $mysqli->query($query);

$data = array();
foreach ($result as $row) {
  $data[] = $row;
}

$result->close();

$mysqli->close();

print json_encode($data);