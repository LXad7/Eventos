<?php
// Configurações da base de dados
define('DB_HOST', 'localhost');
define('DB_USER', 'root');
define('DB_PASS', '');
define('DB_NAME', 'Eventos');
define('DB_PORT', 3306);

// Configurações do site
define('SITE_NAME', 'Arena Fitness');
define('SITE_URL', 'http://localhost/Eventos');
define('UPLOAD_PATH', 'uploads/eventos/');
define('MAX_FILE_SIZE', 5 * 1024 * 1024); // 5MB

// Configurações de sessão
ini_set('session.cookie_lifetime', 3600); // 1 hora
session_start();

// Conexão à base de dados
try {
    $pdo = new PDO(
        "mysql:host=" . DB_HOST . ";port=" . DB_PORT . ";dbname=" . DB_NAME . ";charset=utf8mb4", 
        DB_USER, 
        DB_PASS,
        [
            PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION,
            PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
            PDO::ATTR_EMULATE_PREPARES => false
        ]
    );
} catch(PDOException $e) {
    die("Erro de conexão à base de dados: " . $e->getMessage());
}

// Funções auxiliares
function isLoggedIn() {
    return isset($_SESSION['user_id']);
}

function isAdmin() {
    return isset($_SESSION['user_type']) && $_SESSION['user_type'] === 'administrador';
}

function requireLogin() {
    if (!isLoggedIn()) {
        header('Location: login.php');
        exit;
    }
}

function requireAdmin() {
    if (!isAdmin()) {
        header('Location: index.php');
        exit;
    }
}

function sanitizeInput($input) {
    return htmlspecialchars(trim($input), ENT_QUOTES, 'UTF-8');
}

function formatDate($date, $format = 'd/m/Y H:i') {
    return date($format, strtotime($date));
}

function generateTicketCode() {
    return 'EU' . date('Y') . rand(100000, 999999);
}

function uploadImage($file, $eventId) {
    if (!isset($file) || $file['error'] !== UPLOAD_ERR_OK) {
        return false;
    }
    
    $allowedTypes = ['image/jpeg', 'image/png', 'image/gif'];
    if (!in_array($file['type'], $allowedTypes)) {
        return false;
    }
    
    if ($file['size'] > MAX_FILE_SIZE) {
        return false;
    }
    
    $extension = pathinfo($file['name'], PATHINFO_EXTENSION);
    $filename = 'evento_' . $eventId . '_' . time() . '.' . $extension;
    $uploadPath = UPLOAD_PATH . $filename;
    
    if (move_uploaded_file($file['tmp_name'], $uploadPath)) {
        return $filename;
    }
    
    return false;
}

function showMessage($message, $type = 'info') {
    $_SESSION['message'] = $message;
    $_SESSION['message_type'] = $type;
}

function displayMessage() {
    if (isset($_SESSION['message'])) {
        $message = $_SESSION['message'];
        $type = $_SESSION['message_type'] ?? 'info';
        echo "<div class='alert alert-{$type}'>{$message}</div>";
        unset($_SESSION['message'], $_SESSION['message_type']);
    }
}
?>
