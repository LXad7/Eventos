-- Base de dados para o projeto Eventos Arena
-- Criação da base de dados
CREATE DATABASE IF NOT EXISTS Eventos CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE Eventos;

-- Tabela de utilizadores
CREATE TABLE utilizadores (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(100) NOT NULL,
    email VARCHAR(100) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL,
    telefone VARCHAR(15),
    data_nascimento DATE,
    tipo_utilizador ENUM('utilizador', 'administrador') DEFAULT 'utilizador',
    ativo TINYINT(1) DEFAULT 1,
    data_registo TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    ultimo_login TIMESTAMP NULL
);

-- Tabela de categorias
CREATE TABLE categorias (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(50) NOT NULL UNIQUE,
    descricao TEXT,
    ativo TINYINT(1) DEFAULT 1,
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabela de tipos de eventos
CREATE TABLE tipos_eventos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(50) NOT NULL UNIQUE,
    descricao TEXT,
    ativo TINYINT(1) DEFAULT 1,
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabela de eventos
CREATE TABLE eventos (
    id INT AUTO_INCREMENT PRIMARY KEY,
    titulo VARCHAR(200) NOT NULL,
    descricao TEXT NOT NULL,
    data_evento DATETIME NOT NULL,
    data_fim DATETIME,
    local VARCHAR(200) NOT NULL,
    endereco TEXT,
    cidade VARCHAR(100) NOT NULL,
    distrito VARCHAR(50) NOT NULL,
    codigo_postal VARCHAR(10),
    preco DECIMAL(10,2) DEFAULT 0.00,
    lotacao_maxima INT,
    categoria_id INT NOT NULL,
    tipo_evento_id INT NOT NULL,
    organizador_id INT NOT NULL,
    imagem VARCHAR(255),
    status ENUM('pendente', 'aprovado', 'rejeitado', 'cancelado') DEFAULT 'pendente',
    ativo TINYINT(1) DEFAULT 1,
    data_criacao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    data_aprovacao TIMESTAMP NULL,
    aprovado_por INT NULL,
    FOREIGN KEY (categoria_id) REFERENCES categorias(id),
    FOREIGN KEY (tipo_evento_id) REFERENCES tipos_eventos(id),
    FOREIGN KEY (organizador_id) REFERENCES utilizadores(id),
    FOREIGN KEY (aprovado_por) REFERENCES utilizadores(id)
);

-- Tabela de interesses
CREATE TABLE interesses (
    id INT AUTO_INCREMENT PRIMARY KEY,
    utilizador_id INT NOT NULL,
    evento_id INT NOT NULL,
    data_interesse TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (utilizador_id) REFERENCES utilizadores(id) ON DELETE CASCADE,
    FOREIGN KEY (evento_id) REFERENCES eventos(id) ON DELETE CASCADE,
    UNIQUE KEY unique_interesse (utilizador_id, evento_id)
);

-- Tabela de inscrições
CREATE TABLE inscricoes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    utilizador_id INT NOT NULL,
    evento_id INT NOT NULL,
    quantidade_bilhetes INT DEFAULT 1,
    valor_total DECIMAL(10,2) NOT NULL,
    status ENUM('pendente', 'confirmada', 'cancelada') DEFAULT 'pendente',
    data_inscricao TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    data_pagamento TIMESTAMP NULL,
    codigo_bilhete VARCHAR(50) UNIQUE,
    observacoes TEXT,
    FOREIGN KEY (utilizador_id) REFERENCES utilizadores(id) ON DELETE CASCADE,
    FOREIGN KEY (evento_id) REFERENCES eventos(id) ON DELETE CASCADE
);

-- Inserir dados iniciais

-- Inserir utilizador administrador
INSERT INTO utilizadores (nome, email, password, tipo_utilizador) VALUES 
('Administrador', 'admin@eventosarena.pt', '$2y$10$92IXUNpkjO0rOQ5byMi.Ye4oKoEa3Ro9llC/.og/at2.uheWG/igi', 'administrador');

-- Inserir categorias
INSERT INTO categorias (nome, descricao) VALUES 
('Música', 'Concertos, festivais e eventos musicais'),
('Desporto', 'Eventos desportivos e competições'),
('Arte e Cultura', 'Exposições, teatro e eventos culturais'),
('Tecnologia', 'Conferências e eventos de tecnologia'),
('Gastronomia', 'Eventos gastronómicos e culinários'),
('Educação', 'Workshops, formações e seminários'),
('Negócios', 'Eventos corporativos e networking'),
('Entretenimento', 'Shows, comédia e entretenimento geral');

-- Inserir tipos de eventos
INSERT INTO tipos_eventos (nome, descricao) VALUES 
('Concerto', 'Apresentações musicais ao vivo'),
('Festival', 'Eventos de múltiplos dias com várias atividades'),
('Workshop', 'Sessões práticas de aprendizagem'),
('Conferência', 'Apresentações e palestras'),
('Exposição', 'Mostras de arte, produtos ou serviços'),
('Competição', 'Eventos competitivos'),
('Feira', 'Eventos comerciais e de vendas'),
('Espetáculo', 'Apresentações teatrais ou de entretenimento');
