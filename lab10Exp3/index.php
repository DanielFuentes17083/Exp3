<?php
header("Content-type: text/html; charset=\"utf-8\"");
$carnet = 0;
$id_color = 0;
$color = 0;
//print_r($_POST);
if($_POST){
	$carnet = $_POST['carnet'];
	$id_color = $_POST['id_color'];
    $color = $_POST['color'];
	
    $conexion = mysqli_connect("localhost:3306","root","", "color");
    if(!$conexion){
        echo "Error: No se pudo conectar a MySQL.".PHP_EOL;
        echo "errno de depuración: ". mysqli_connect_errno().PHP_EOL;
        echo "error de depuración: ". mysqli_connect_error().PHP_EOL;
        
    }else{
        $query = "INSERT INTO `datos` (`id`, `fecha`, `carnet`, `id_color`, `color`) VALUES (NULL, CURRENT_TIMESTAMP, '$carnet', '$id_color', '$color');";
        mysqli_query($conexion, $query);
	
    }
	mysqli_close($conexion);
}

$conexion = mysqli_connect("localhost:3306","root","", "color");
if(!$conexion){
    echo "Error: No se pudo conectar a MySQL.".PHP_EOL;
    echo "errno de depuración: ". mysqli_connect_errno().PHP_EOL;
    echo "error de depuración: ". mysqli_connect_error().PHP_EOL;
}else{
    $query = "SELECT * FROM `datos` ORDER BY id DESC LIMIT 1";
    $result = mysqli_query($conexion,$query);
	$fecha =1;
}
mysqli_close($conexion);

    
?>
<!doctype html>
<html lang="en">

<head>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

  <!-- Bootstrap CSS -->
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css"
    integrity="sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk" crossorigin="anonymous">

  <title>Lab 10 Exp 3</title>
  
</head>
<?php
$conexion = mysqli_connect("localhost:3306","root","", "color");
if(!$conexion){
  echo "Error: No se pudo conectar a MySQL.".PHP_EOL;
  echo "errno de depuración: ". mysqli_connect_errno().PHP_EOL;
  echo "error de depuración: ". mysqli_connect_error().PHP_EOL;
}else{
  $query = "SELECT id_color FROM `datos` ORDER BY id DESC LIMIT 1";
  $result2 = mysqli_query($conexion,$query);
  while($fila = mysqli_fetch_array($result2)){
    $ultimo_color =  $fila['id_color'];
  }
}
mysqli_close($conexion);
if ($ultimo_color == 0){
echo sprintf('<style type="text/css">
body{
  background-color:black
}
</style>');
}
if ($ultimo_color == 1){
echo sprintf('<style type="text/css">
body{
  background-color:blue
}
</style>');
}
if ($ultimo_color == 2){
echo sprintf('<style type="text/css">
body{
  background-color:green
}
</style>');
}
if ($ultimo_color == 3){
echo sprintf('<style type="text/css">
body{
  background-color:cyan
}
</style>');
}
if ($ultimo_color == 4){
echo sprintf('<style type="text/css">
body{
  background-color:red
}
</style>');
}
if ($ultimo_color == 5){
echo sprintf('<style type="text/css">
body{
  background-color:fuchsia
}
</style>');
}
if ($ultimo_color == 6){
echo sprintf('<style type="text/css">
body{
  background-color:yellow
}
</style>');
}
if ($ultimo_color == 7){
echo sprintf('<style type="text/css">
body{
  background-color:white
}
</style>');
}
?>


  <div class='container'>
    <br>
    <h1><strong>LEDs Color</strong></h1>
    <br>

    <table class="table table-light">

      <thead class="bg-success text-white">
        <tr>
          <th>Fecha</th>
          <th>Carnet</th>
		  <th>id_color</th>
		  <th>Color</th>
        </tr>
      </thead>
      <tbody>
        <?php
                    while($fila = mysqli_fetch_array($result)){
                      echo "<tr>
                                <td>".$fila['fecha']."</td>
                                <td>".$fila['carnet']."</td>
                                <td>".$fila['id_color']."</td>
                                <td>".$fila['color']."</td>
                           </tr>";       
                        
                    }
                ?>
      </tbody>
    </table>
    <br>
    <div class="container" id="formulariodiv">
      <form id="formulario" action="index.php" method="POST" accept-charset="utf-8">
        <br><h2> Formulario </h2>
        
		<br> Carnet: <br>
        <input type="text" name="carnet" id="carnet">
        <br> id_color: <br>
        <input type="text" name="id_color" id="id_color">
		<br> Color: <br>
        <input type="text" name="color" id="color">
        <br>
        <br><input type="submit" value="Enviar">
      </form>
    </div>

    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"
      integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj"
      crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js"
      integrity="sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo"
      crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js"
      integrity="sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI"
      crossorigin="anonymous"></script>
</body>

</html>