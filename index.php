<?php
require_once 'includes/config.php';

// Buscar eventos aprovados
$searchQuery = $_GET['search'] ?? '';
$categoriaId = $_GET['categoria'] ?? '';
$tipoId = $_GET['tipo'] ?? '';
$cidade = $_GET['cidade'] ?? '';

$sql = "SELECT e.*, c.nome as categoria_nome, t.nome as tipo_nome, u.nome as organizador_nome 
        FROM eventos e 
        JOIN categorias c ON e.categoria_id = c.id 
        JOIN tipos_eventos t ON e.tipo_evento_id = t.id 
        JOIN utilizadores u ON e.organizador_id = u.id 
        WHERE e.status = 'aprovado' AND e.ativo = 1 AND e.data_evento >= NOW()";

$params = [];

if ($searchQuery) {
    $sql .= " AND (e.titulo LIKE ? OR e.descricao LIKE ?)";
    $params[] = "%$searchQuery%";
    $params[] = "%$searchQuery%";
}

if ($categoriaId) {
    $sql .= " AND e.categoria_id = ?";
    $params[] = $categoriaId;
}

if ($tipoId) {
    $sql .= " AND e.tipo_evento_id = ?";
    $params[] = $tipoId;
}

if ($cidade) {
    $sql .= " AND e.cidade LIKE ?";
    $params[] = "%$cidade%";
}

$sql .= " ORDER BY e.data_evento ASC LIMIT 12";

$stmt = $pdo->prepare($sql);
$stmt->execute($params);
$eventos = $stmt->fetchAll();

// Buscar categorias para filtro
$categorias = $pdo->query("SELECT * FROM categorias WHERE ativo = 1 ORDER BY nome")->fetchAll();

// Buscar tipos para filtro
$tipos = $pdo->query("SELECT * FROM tipos_eventos WHERE ativo = 1 ORDER BY nome")->fetchAll();
?>
<!DOCTYPE html>
<html lang="pt-PT">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?= SITE_NAME ?> - Eventos em Portugal</title>
    <link rel="stylesheet" href="css/style.css">
</head>
<body>
    <header>
        <nav class="navbar">
            <div class="nav-container">
                <div class="nav-logo">
                    <h1><a href="index.php"><?= SITE_NAME ?></a></h1>
                </div>
                <div class="nav-menu">
                    <a href="index.php">Início</a>
                    <a href="pages/eventos.php">Eventos</a>
                    <?php if (isLoggedIn()): ?>
                        <a href="pages/meus-eventos.php">Meus Eventos</a>
                        <a href="pages/criar-evento.php">Criar Evento</a>
                        <?php if (isAdmin()): ?>
                            <a href="admin/dashboard.php">Administração</a>
                        <?php endif; ?>
                        <a href="pages/perfil.php"><?= $_SESSION['user_name'] ?></a>
                        <a href="logout.php">Sair</a>
                    <?php else: ?>
                        <a href="login.php">Login</a>
                        <a href="registo.php">Registar</a>
                    <?php endif; ?>
                </div>
            </div>
        </nav>
    </header>

    <main>
        <!-- Hero Section -->
        <section class="hero">
            <div class="hero-content">
                <h1>Descubra Eventos Incríveis no Arena Fitness</h1>
                <p>A sua plataforma para encontrar, criar e participar nos melhores eventos fitness da Figueira da Foz</p>
                
                <!-- Formulário de pesquisa -->
                <form class="search-form" method="GET" action="">
                    <div class="search-row">
                        <input type="text" name="search" placeholder="Pesquisar eventos..." 
                               value="<?= htmlspecialchars($searchQuery) ?>">
                        
                        <select name="categoria">
                            <option value="">Todas as categorias</option>
                            <?php foreach ($categorias as $categoria): ?>
                                <option value="<?= $categoria['id'] ?>" 
                                        <?= $categoriaId == $categoria['id'] ? 'selected' : '' ?>>
                                    <?= htmlspecialchars($categoria['nome']) ?>
                                </option>
                            <?php endforeach; ?>
                        </select>
                        
                        <input type="text" name="cidade" placeholder="Cidade..." 
                               value="<?= htmlspecialchars($cidade) ?>">
                        
                        <button type="submit" class="btn btn-primary">Pesquisar</button>
                    </div>
                </form>
            </div>
        </section>

        <!-- Eventos em Destaque -->
        <section class="eventos-section">
            <div class="container">
                <h2>Próximos Eventos</h2>
                
                <?php if (empty($eventos)): ?>
                    <div class="no-events">
                        <p>Nenhum evento encontrado com os critérios selecionados.</p>
                        <?php if (isLoggedIn()): ?>
                            <a href="pages/criar-evento.php" class="btn btn-primary">Criar Primeiro Evento</a>
                        <?php endif; ?>
                    </div>
                <?php else: ?>
                    <div class="eventos-grid">
                        <?php foreach ($eventos as $evento): ?>
                            <div class="evento-card">
                                <?php if ($evento['imagem']): ?>
                                    <img src="<?= UPLOAD_PATH . $evento['imagem'] ?>" 
                                         alt="<?= htmlspecialchars($evento['titulo']) ?>">
                                <?php else: ?>
                                    <div class="no-image">Sem imagem</div>
                                <?php endif; ?>
                                
                                <div class="evento-content">
                                    <h3><?= htmlspecialchars($evento['titulo']) ?></h3>
                                    <p class="evento-data"><?= formatDate($evento['data_evento']) ?></p>
                                    <p class="evento-local"><?= htmlspecialchars($evento['local'] . ', ' . $evento['cidade']) ?></p>
                                    <p class="evento-categoria"><?= htmlspecialchars($evento['categoria_nome']) ?></p>
                                    
                                    <?php if ($evento['preco'] > 0): ?>
                                        <p class="evento-preco">€<?= number_format($evento['preco'], 2) ?></p>
                                    <?php else: ?>
                                        <p class="evento-preco gratis">Grátis</p>
                                    <?php endif; ?>
                                    
                                    <a href="pages/evento.php?id=<?= $evento['id'] ?>" 
                                       class="btn btn-secondary">Ver Detalhes</a>
                                </div>
                            </div>
                        <?php endforeach; ?>
                    </div>
                    
                    <div class="ver-mais">
                        <a href="pages/eventos.php" class="btn btn-outline">Ver Todos os Eventos</a>
                    </div>
                <?php endif; ?>
            </div>
        </section>

        <!-- Estatísticas -->
        <section class="stats-section">
            <div class="container">
                <h2>Arena Fitness em Números</h2>
                <div class="stats-grid">
                    <?php
                    $totalEventos = $pdo->query("SELECT COUNT(*) FROM eventos WHERE status = 'aprovado'")->fetchColumn();
                    $totalUsuarios = $pdo->query("SELECT COUNT(*) FROM utilizadores WHERE ativo = 1")->fetchColumn();
                    $totalInscricoes = $pdo->query("SELECT COUNT(*) FROM inscricoes WHERE status = 'confirmada'")->fetchColumn();
                    ?>
                    
                    <div class="stat-item">
                        <h3><?= $totalEventos ?></h3>
                        <p>Eventos Realizados</p>
                    </div>
                    <div class="stat-item">
                        <h3><?= $totalUsuarios ?></h3>
                        <p>Utilizadores Registados</p>
                    </div>
                    <div class="stat-item">
                        <h3><?= $totalInscricoes ?></h3>
                        <p>Inscrições Confirmadas</p>
                    </div>
                </div>
            </div>
        </section>
    </main>

    <footer>
        <div class="container">
            <div class="footer-content">
                <div class="footer-section">
                    <h3><?= SITE_NAME ?></h3>
                    <p>A sua plataforma de eventos fitness na Figueira da Foz</p>
                </div>
                <div class="footer-section">
                    <h4>Links Úteis</h4>
                    <ul>
                        <li><a href="pages/eventos.php">Eventos</a></li>
                        <li><a href="pages/criar-evento.php">Criar Evento</a></li>
                        <li><a href="pages/sobre.php">Sobre Nós</a></li>
                        <li><a href="pages/contacto.php">Contacto</a></li>
                    </ul>
                </div>
                <div class="footer-section">
                    <h4>Contacto</h4>
                    <p>Email: info@arenafitnessfigueira.pt</p>
                    <p>Telefone: +351 123 456 789</p>
                </div>
            </div>
            <div class="footer-bottom">
                <p>&copy; <?= date('Y') ?> <?= SITE_NAME ?>. Todos os direitos reservados.</p>
            </div>
        </div>
    </footer>

    <script src="js/main.js"></script>
</body>
</html>
