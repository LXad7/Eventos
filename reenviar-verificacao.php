<?php
require_once 'includes/config.php';

$message = '';
$success = false;

if ($_POST) {
    $email = sanitizeInput($_POST['email'] ?? '');
    
    if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        $message = 'Email inválido.';
    } else {
        try {
            // Buscar utilizador não verificado
            $stmt = $pdo->prepare("
                SELECT id, nome, email 
                FROM utilizadores 
                WHERE email = ? 
                AND email_verificado = 0 
                AND ativo = 1
            ");
            $stmt->execute([$email]);
            $user = $stmt->fetch();

            if ($user) {
                // Gerar novo token
                $newToken = generateVerificationToken();
                
                // Atualizar token na base de dados
                $stmt = $pdo->prepare("
                    UPDATE utilizadores 
                    SET token_verificacao = ?, data_token = NOW() 
                    WHERE id = ?
                ");
                $stmt->execute([$newToken, $user['id']]);
                
                // Enviar novo email
                if (sendVerificationEmail($user['email'], $user['nome'], $newToken)) {
                    $message = 'Email de verificação reenviado com sucesso!';
                    $success = true;
                } else {
                    $message = 'Erro ao enviar email. Tente novamente mais tarde.';
                }
            } else {
                $message = 'Email não encontrado ou já verificado.';
            }
        } catch (PDOException $e) {
            $message = 'Erro no sistema. Tente novamente mais tarde.';
        }
    }
}
?>
<!DOCTYPE html>
<html lang="pt-PT">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Reenviar Verificação - <?= SITE_NAME ?></title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="login-container">
        <div class="login-form">
            <div class="logo">
                <h1><?= SITE_NAME ?></h1>
                <p>Reenviar Email de Verificação</p>
            </div>
            
            <?php if ($message): ?>
                <div class="alert <?= $success ? 'alert-success' : 'alert-error' ?>">
                    <p><?= htmlspecialchars($message) ?></p>
                </div>
            <?php endif; ?>
            
            <?php if (!$success): ?>
            <form method="POST" action="">
                <div class="form-group">
                    <label for="email">Email:</label>
                    <input type="email" id="email" name="email" required 
                           value="<?= htmlspecialchars($email ?? '') ?>"
                           placeholder="Digite o seu email">
                </div>
                
                <button type="submit" class="btn btn-primary">Reenviar Verificação</button>
            </form>
            <?php endif; ?>
            
            <div class="login-links">
                <p><a href="login.php">Voltar ao Login</a></p>
                <p><a href="registo.php">Criar Nova Conta</a></p>
                <p><a href="index.php">Voltar ao início</a></p>
            </div>
        </div>
    </div>
</body>
</html>
