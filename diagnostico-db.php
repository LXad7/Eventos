<?php
require_once 'includes/config.php';

echo "<h1>Diagnóstico da Base de Dados - " . SITE_NAME . "</h1>";
echo "<hr>";

try {
    // Listar todas as tabelas
    echo "<h2>📋 Tabelas na Base de Dados:</h2>";
    $stmt = $pdo->query("SHOW TABLES");
    $tables = $stmt->fetchAll(PDO::FETCH_COLUMN);
    
    echo "<ol>";
    foreach ($tables as $table) {
        echo "<li><strong>" . $table . "</strong></li>";
    }
    echo "</ol>";
    
    echo "<h2>🔍 Análise de Possíveis Duplicações:</h2>";
    
    // Identificar possíveis duplicações
    $duplicates = [];
    $singulars = [];
    
    foreach ($tables as $table) {
        // Verificar se existe versão singular/plural
        if (substr($table, -1) === 's') {
            $singular = substr($table, 0, -1);
            if (in_array($singular, $tables)) {
                $duplicates[] = ['plural' => $table, 'singular' => $singular];
            }
        }
    }
    
    if (!empty($duplicates)) {
        echo "<div style='background: #fff3cd; padding: 15px; border: 1px solid #ffeaa7; border-radius: 5px; margin: 10px 0;'>";
        echo "<h3>⚠️ Tabelas Duplicadas Encontradas:</h3>";
        foreach ($duplicates as $dup) {
            echo "<p>";
            echo "<strong>Plural:</strong> " . $dup['plural'] . " | ";
            echo "<strong>Singular:</strong> " . $dup['singular'];
            echo "</p>";
        }
        echo "</div>";
    }
    
    echo "<h2>📊 Estrutura e Conteúdo das Tabelas:</h2>";
    
    foreach ($tables as $table) {
        echo "<h3>Tabela: <code>$table</code></h3>";
        
        // Mostrar estrutura
        echo "<h4>Estrutura:</h4>";
        $stmt = $pdo->query("DESCRIBE $table");
        $columns = $stmt->fetchAll();
        
        echo "<table border='1' style='border-collapse: collapse; width: 100%; margin-bottom: 20px;'>";
        echo "<tr style='background: #f8f9fa;'><th>Campo</th><th>Tipo</th><th>Null</th><th>Chave</th><th>Default</th><th>Extra</th></tr>";
        foreach ($columns as $col) {
            echo "<tr>";
            echo "<td>" . $col['Field'] . "</td>";
            echo "<td>" . $col['Type'] . "</td>";
            echo "<td>" . $col['Null'] . "</td>";
            echo "<td>" . $col['Key'] . "</td>";
            echo "<td>" . $col['Default'] . "</td>";
            echo "<td>" . $col['Extra'] . "</td>";
            echo "</tr>";
        }
        echo "</table>";
        
        // Mostrar número de registos
        $stmt = $pdo->query("SELECT COUNT(*) as total FROM $table");
        $count = $stmt->fetch();
        echo "<p><strong>Registos:</strong> " . $count['total'] . "</p>";
        
        // Se tem poucos registos, mostrar conteúdo
        if ($count['total'] <= 10 && $count['total'] > 0) {
            echo "<h4>Conteúdo:</h4>";
            $stmt = $pdo->query("SELECT * FROM $table LIMIT 10");
            $rows = $stmt->fetchAll();
            
            if (!empty($rows)) {
                echo "<table border='1' style='border-collapse: collapse; width: 100%; margin-bottom: 20px; font-size: 12px;'>";
                echo "<tr style='background: #f8f9fa;'>";
                foreach (array_keys($rows[0]) as $header) {
                    echo "<th>$header</th>";
                }
                echo "</tr>";
                
                foreach ($rows as $row) {
                    echo "<tr>";
                    foreach ($row as $value) {
                        echo "<td>" . htmlspecialchars(substr($value, 0, 50)) . (strlen($value) > 50 ? '...' : '') . "</td>";
                    }
                    echo "</tr>";
                }
                echo "</table>";
            }
        }
        
        echo "<hr>";
    }
    
} catch (PDOException $e) {
    echo "<div style='color: red; background: #ffe6e6; padding: 15px; border-radius: 5px;'>";
    echo "<h3>❌ Erro ao acessar a base de dados:</h3>";
    echo "<p>" . $e->getMessage() . "</p>";
    echo "<p><strong>Possíveis causas:</strong></p>";
    echo "<ul>";
    echo "<li>Base de dados 'Eventos' não existe</li>";
    echo "<li>Configurações incorretas no config.php</li>";
    echo "<li>MySQL não está a correr</li>";
    echo "</ul>";
    echo "</div>";
}
?>

<h2>🛠️ Recomendações:</h2>
<div style="background: #e8f5e8; padding: 15px; border-radius: 5px; margin: 10px 0;">
    <ol>
        <li><strong>Execute primeiro:</strong> <code>database.sql</code> para criar a estrutura</li>
        <li><strong>Se houver duplicações:</strong> Elimine as tabelas desnecessárias</li>
        <li><strong>Execute depois:</strong> <code>update_verification.sql</code> para adicionar campos de verificação</li>
        <li><strong>Execute por fim:</strong> <code>dados-demo.sql</code> para dados de teste</li>
    </ol>
</div>

<p><a href="index.php">← Voltar ao site</a></p>
