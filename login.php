<?php
require_once 'includes/config.php';

// Se já está logado, redirecionar
if (isLoggedIn()) {
    header('Location: index.php');
    exit;
}

$error = '';

if ($_POST) {
    $email = sanitizeInput($_POST['email'] ?? '');
    $password = $_POST['password'] ?? '';
    
    if ($email && $password) {
        try {
            $stmt = $pdo->prepare("SELECT id, nome, email, password, tipo_utilizador, ativo FROM utilizadores WHERE email = ? AND ativo = 1");
            $stmt->execute([$email]);
            $user = $stmt->fetch();
            
            if ($user && password_verify($password, $user['password'])) {
                // Login válido
                $_SESSION['user_id'] = $user['id'];
                $_SESSION['user_name'] = $user['nome'];
                $_SESSION['user_email'] = $user['email'];
                $_SESSION['user_type'] = $user['tipo_utilizador'];
                
                // Atualizar último login
                $updateStmt = $pdo->prepare("UPDATE utilizadores SET ultimo_login = NOW() WHERE id = ?");
                $updateStmt->execute([$user['id']]);
                
                // Redirecionar
                if ($user['tipo_utilizador'] === 'administrador') {
                    header('Location: admin/dashboard.php');
                } else {
                    header('Location: index.php');
                }
                exit;
            } else {
                $error = 'Email ou password incorretos.';
            }
        } catch (PDOException $e) {
            $error = 'Erro no sistema. Tente novamente.';
        }
    } else {
        $error = 'Por favor, preencha todos os campos.';
    }
}
?>
<!DOCTYPE html>
<html lang="pt-PT">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login - <?= SITE_NAME ?></title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <div class="login-container">
        <div class="login-form">
            <div class="logo">
                <h1><?= SITE_NAME ?></h1>
                <p>Sistema de Gestão de Eventos</p>
            </div>
            
            <?php if ($error): ?>
                <div class="alert alert-error"><?= $error ?></div>
            <?php endif; ?>
            
            <form method="POST" action="">
                <div class="form-group">
                    <label for="email">Email:</label>
                    <input type="email" id="email" name="email" required 
                           value="<?= htmlspecialchars($_POST['email'] ?? '') ?>">
                </div>
                
                <div class="form-group">
                    <label for="password">Password:</label>
                    <input type="password" id="password" name="password" required>
                </div>
                
                <button type="submit" class="btn btn-primary">Entrar</button>
            </form>
            
            <div class="login-links">
                <p>Não tem conta? <a href="registo.php">Registar-se</a></p>
                <p><a href="index.php">Voltar ao início</a></p>
            </div>
            
            <div class="demo-accounts">
                <h4>Contas de demonstração:</h4>
                <p><strong>Admin:</strong> admin@eventosarena.pt / password</p>
                <p><strong>Utilizador:</strong> user@eventosarena.pt / password</p>
            </div>
        </div>
    </div>
</body>
</html>
