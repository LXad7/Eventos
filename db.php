<?php
// Incluir configurações
require_once 'includes/config.php';

// Manter a conexão mysqli para compatibilidade
$servidor = DB_HOST;
$utilizador = DB_USER;         
$senha = DB_PASS;      
$basedados = DB_NAME;       
$porta = DB_PORT;               

$conn = new mysqli($servidor, $utilizador, $senha, $basedados, $porta);

if ($conn->connect_error) {
    die("Erro de ligação à base de dados: " . $conn->connect_error);
}

// Definir charset
$conn->set_charset("utf8mb4");
?>
