<?php


$servername = "localhost";
$dbname = "id19883322_adatok";
$username = "id19883322_admin";
$password = "*}Nv)[y6)G5dt[3Ij";

$api_key_value = "d7a03fee5546592a37e22ff8f45bbbe45da4632dfed9a774e085d0e8b5d3fa73";

$api_key = $light = $collor = $moisture = $humidity = $temperature = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if ($api_key == $api_key_value) {
        $humidity = test_input($_POST["HUM"]);
        $temperature = test_input($_POST["TEMP"]);
        $moisture = test_input($_POST["MOIST"]);
        $light = test_input($_POST["LIGHT"]);
        $collor = test_input($_POST["COLLOR"]);




        $connection = new mysqli($servername, $username, $password, $dbname);

        if ($connection->connect_error) {
            die("Connection failed: " . $connection->connect_error);
        }

        $sql = "INSERT INTO arduino (HUM, TEMP, MOIST, LIGHT, COLLOR) VALUES ('" . $humidity . "', '" . $temperature . "', '" . $moisture . "', '" . $light . "', '" . $collor . "')";

        if ($connection->query($sql) === TRUE) {
            echo "New record created successfully";
        } else {
            echo "Error: " . $sql . "<br>" . $connection->error;
        }

        $connection->close();
    } else {
        echo "Wrong API Key provided.";
    }

} else {
    echo "No data posted with HTTP POST.";
}

function test_input($data)
{
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}