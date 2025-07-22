<?php
require_once 'includes/config.php';

// Se já está logado, redirecionar
if (isLoggedIn()) {
    header('Location: index.php');
    exit;
}

$errors = [];
$success = '';

if ($_POST) {
    $nome = sanitizeInput($_POST['nome'] ?? '');
    $email = sanitizeInput($_POST['email'] ?? '');
    $telefone = sanitizeInput($_POST['telefone'] ?? '');
    $data_nascimento = $_POST['data_nascimento'] ?? '';
    $password = $_POST['password'] ?? '';
    $confirm_password = $_POST['confirm_password'] ?? '';
    
    // Validações
    if (strlen($nome) < 2) {
        $errors[] = 'Nome deve ter pelo menos 2 caracteres.';
    }
    
    if (!filter_var($email, FILTER_VALIDATE_EMAIL)) {
        $errors[] = 'Email inválido.';
    }
    
    if (strlen($password) < 6) {
        $errors[] = 'Password deve ter pelo menos 6 caracteres.';
    }
    
    if ($password !== $confirm_password) {
        $errors[] = 'Passwords não coincidem.';
    }
    
    if (empty($errors)) {
        try {
            // Verificar se email já existe
            $stmt = $pdo->prepare("SELECT id FROM utilizadores WHERE email = ?");
            $stmt->execute([$email]);
            if ($stmt->fetch()) {
                $errors[] = 'Email já registado.';
            } else {
                // Gerar token de verificação
                $verificationToken = generateVerificationToken();
                
                // Inserir novo utilizador com email não verificado
                $hashedPassword = password_hash($password, PASSWORD_DEFAULT);
                $stmt = $pdo->prepare("
                    INSERT INTO utilizadores 
                    (nome, email, telefone, data_nascimento, password, email_verificado, token_verificacao, data_token) 
                    VALUES (?, ?, ?, ?, ?, 0, ?, NOW())
                ");
                $stmt->execute([$nome, $email, $telefone, $data_nascimento, $hashedPassword, $verificationToken]);
                
                // Enviar email de verificação
                if (sendVerificationEmail($email, $nome, $verificationToken)) {
                    $success = 'Registo efetuado com sucesso! Verifique o seu email para ativar a conta.';
                } else {
                    $success = 'Registo efetuado! Não foi possível enviar o email de verificação. Contacte o suporte.';
                }
                
                // Limpar campos
                $nome = $email = $telefone = $data_nascimento = '';
            }
        } catch (PDOException $e) {
            $errors[] = 'Erro no sistema. Tente novamente.';
        }
    }
}
?>
<!DOCTYPE html>
<html lang="pt-PT">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Registo - <?= SITE_NAME ?></title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="login-container">
        <div class="login-form register-form">
            <div class="logo">
                <h1><?= SITE_NAME ?></h1>
                <p>Criar Nova Conta</p>
            </div>
            
            <?php if ($errors): ?>
                <div class="alert alert-error">
                    <?php foreach ($errors as $error): ?>
                        <p><?= $error ?></p>
                    <?php endforeach; ?>
                </div>
            <?php endif; ?>
            
            <?php if ($success): ?>
                <div class="alert alert-success"><?= $success ?></div>
            <?php endif; ?>
            
            <form method="POST" action="">
                <div class="form-group">
                    <label for="nome">Nome Completo:</label>
                    <input type="text" id="nome" name="nome" required 
                           value="<?= htmlspecialchars($nome ?? '') ?>">
                </div>
                
                <div class="form-group">
                    <label for="email">Email:</label>
                    <input type="email" id="email" name="email" required 
                           value="<?= htmlspecialchars($email ?? '') ?>">
                </div>
                
                <div class="form-group">
                    <label for="telefone">Telefone:</label>
                    <input type="tel" id="telefone" name="telefone" 
                           value="<?= htmlspecialchars($telefone ?? '') ?>">
                </div>
                
                <div class="form-group">
                    <label for="data_nascimento">Data de Nascimento:</label>
                    <input type="date" id="data_nascimento" name="data_nascimento" 
                           value="<?= htmlspecialchars($data_nascimento ?? '') ?>">
                </div>
                
                <div class="form-group">
                    <label for="password">Password:</label>
                    <input type="password" id="password" name="password" required>
                </div>
                
                <div class="form-group">
                    <label for="confirm_password">Confirmar Password:</label>
                    <input type="password" id="confirm_password" name="confirm_password" required>
                </div>
                
                <button type="submit" class="btn btn-primary">Registar</button>
            </form>
            
            <div class="login-links">
                <p>Já tem conta? <a href="login.php">Fazer login</a></p>
                <p><a href="index.php">Voltar ao início</a></p>
            </div>
        </div>
    </div>
</body>
</html>
