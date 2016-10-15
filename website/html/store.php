<?php
  $servername = "localhost";
  $username = "esp8266";
  $password = "secretpwd";
  $dbname = "esp8266";

  $now = new DateTime();
  parse_str( html_entity_decode( $_SERVER['QUERY_STRING']) , $out);

  if ( array_key_exists( 'temperature' , $out ) ) {
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }

  $datenow = $now->format('Y-m-d H:i:s');
  $tvalue  = $out['temperature'];
  $hvalue  = $out['humidity'];
//  $svalue  = $out['sound'];

//  $sql = "INSERT INTO DataTable ( logdate , temperature  , humidity , sound) VALUES ( '$datenow' , $tvalue, $hvalue, $svalue )";
  $sql = "INSERT INTO DataTable ( logdate , temperature  , humidity ) VALUES ( '$datenow' , $tvalue, $hvalue )";

  if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
  } else {
    echo "Error: " . $sql . "<br>" . $conn->error;
  }

  $conn->close();
}

  $servername = "traflagar16.ctqwzbfptqv3.eu-west-1.rds.amazonaws.com";
  $username = "blair";
  $password = "zep2.8arn";
  $dbname = "trafalgar16";

  $now = new DateTime();
  parse_str( html_entity_decode( $_SERVER['QUERY_STRING']) , $out);

  if ( array_key_exists( 'temperature' , $out ) ) {
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }

  $datenow = $now->format('Y-m-d H:i:s');
  $tvalue  = $out['temperature'];
  $hvalue  = $out['humidity'];
//  $svalue  = $out['sound'];

//  $sql = "INSERT INTO DataTable ( logdate , temperature  , humidity , sound) VALUES ( '$datenow' , $tvalue, $hvalue, $svalue )";
  $sql = "INSERT INTO DataTable ( logdate , temperature  , humidity ) VALUES ( '$datenow' , $tvalue, $hvalue )";

  if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
  } else {
    echo "Error: " . $sql . "<br>" . $conn->error;
  }

  $conn->close();
}
?>
