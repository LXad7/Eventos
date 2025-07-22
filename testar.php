<?php
require_once 'includes/config.php';

echo "<h1>Teste de Conexão - Eventos Arena</h1>";

// Testar conexão PDO
try {
    $stmt = $pdo->query("SELECT COUNT(*) as total FROM utilizadores");
    $result = $stmt->fetch();
    echo "<p style='color: green;'>✅ Conexão PDO: OK</p>";
    echo "<p>Total de utilizadores: " . $result['total'] . "</p>";
} catch (Exception $e) {
    echo "<p style='color: red;'>❌ Erro PDO: " . $e->getMessage() . "</p>";
}

// Testar conexão mysqli
try {
    $result = $conn->query("SELECT COUNT(*) as total FROM categorias");
    if ($result) {
        $row = $result->fetch_assoc();
        echo "<p style='color: green;'>✅ Conexão MySQLi: OK</p>";
        echo "<p>Total de categorias: " . $row['total'] . "</p>";
    }
} catch (Exception $e) {
    echo "<p style='color: red;'>❌ Erro MySQLi: " . $e->getMessage() . "</p>";
}

echo "<p><a href='index.php'>Voltar ao início</a></p>";
?>
