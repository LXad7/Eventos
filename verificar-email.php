<?php
require_once 'includes/config.php';

$message = '';
$success = false;

// Verificar se existe token na URL
$token = $_GET['token'] ?? '';

if ($token) {
    try {
        // Buscar utilizador com este token válido (token criado há menos de 24 horas)
        $stmt = $pdo->prepare("
            SELECT id, nome, email 
            FROM utilizadores 
            WHERE token_verificacao = ? 
            AND email_verificado = 0 
            AND data_token > DATE_SUB(NOW(), INTERVAL 24 HOUR)
        ");
        $stmt->execute([$token]);
        $user = $stmt->fetch();

        if ($user) {
            // Verificar email e limpar token
            $stmt = $pdo->prepare("
                UPDATE utilizadores 
                SET email_verificado = 1, 
                    token_verificacao = NULL, 
                    data_token = NULL 
                WHERE id = ?
            ");
            $stmt->execute([$user['id']]);

            $message = 'Email verificado com sucesso! Pode agora fazer login.';
            $success = true;
        } else {
            $message = 'Token inválido ou expirado. Solicite um novo email de verificação.';
        }
    } catch (PDOException $e) {
        $message = 'Erro no sistema. Tente novamente mais tarde.';
    }
} else {
    $message = 'Token de verificação não fornecido.';
}
?>
<!DOCTYPE html>
<html lang="pt-PT">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Verificação de Email - <?= SITE_NAME ?></title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="login-container">
        <div class="login-form">
            <div class="logo">
                <h1><?= SITE_NAME ?></h1>
                <p>Verificação de Email</p>
            </div>
            
            <div class="alert <?= $success ? 'alert-success' : 'alert-error' ?>">
                <p><?= htmlspecialchars($message) ?></p>
            </div>
            
            <div class="login-links">
                <?php if ($success): ?>
                    <p><a href="login.php" class="btn btn-primary">Fazer Login</a></p>
                <?php else: ?>
                    <p><a href="registo.php" class="btn btn-secondary">Registar Novamente</a></p>
                <?php endif; ?>
                <p><a href="index.php">Voltar ao início</a></p>
            </div>
        </div>
    </div>
</body>
</html>
