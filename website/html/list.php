<?php
$servername = "localhost";
$username = "esp8266";
$password = "secretpwd";
$dbname = "esp8266";

  parse_str( html_entity_decode( $_SERVER['QUERY_STRING']) , $out);

  $sqlq = "Select * from DataTable ";

  if ( array_key_exists( 'field' , $out ) ) {
    $sqlq = $sqlq . " where field = '" . $out['field'] . "'" ;
  }

  echo $sqlq;
  // Create connection
  $conn = new mysqli($servername, $username, $password, $dbname);
  // Check connection
  if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
  }

  $result = mysqli_query( $conn , $sqlq );

  if ( $result->num_rows  > 0 ) {

  echo "<table border='1'>
    <tr>
    <th>Log Date</th>
    <th>Temperature (C)</th>
    <th>Humidity (%)</th>
    <th>Sound</th>
   </tr>";

  while($row = mysqli_fetch_array($result)) {
    echo "<tr>";
    echo "<td>" . $row['logdate'] . "</td>";
    echo "<td>" . $row['temperature'] . "</td>";
    echo "<td>" . $row['humidity'] . "</td>";
    echo "<td>" . $row['sound'] . "</td>";
    echo "</tr>";
  }
  echo "</table>";
}
$conn->close();

?>
