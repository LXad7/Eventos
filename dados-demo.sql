-- Dados de demonstração para o Eventos Arena
-- Execute este script após criar a estrutura da base de dados

USE Eventos;

-- Inserir utilizador de demonstração
INSERT INTO utilizadores (nome, email, password, telefone, data_nascimento, tipo_utilizador) VALUES 
('João Silva', 'user@eventosarena.pt', '$2y$10$92IXUNpkjO0rOQ5byMi.Ye4oKoEa3Ro9llC/.og/at2.uheWG/igi', '912345678', '1990-05-15', 'utilizador'),
('Maria Santos', 'maria@eventosarena.pt', '$2y$10$92IXUNpkjO0rOQ5byMi.Ye4oKoEa3Ro9llC/.og/at2.uheWG/igi', '923456789', '1985-08-22', 'utilizador'),
('Pedro Costa', 'pedro@eventosarena.pt', '$2y$10$92IXUNpkjO0rOQ5byMi.Ye4oKoEa3Ro9llC/.og/at2.uheWG/igi', '934567890', '1992-03-10', 'utilizador');

-- Inserir eventos de demonstração
INSERT INTO eventos (titulo, descricao, data_evento, data_fim, local, endereco, cidade, distrito, codigo_postal, preco, lotacao_maxima, categoria_id, tipo_evento_id, organizador_id, status) VALUES 

('Festival de Verão Lisboa 2025', 'O maior festival de música do verão português com artistas nacionais e internacionais. Três dias de música, diversão e experiências únicas na capital.', '2025-07-15 18:00:00', '2025-07-17 23:59:00', 'Altice Arena', 'Rossio dos Olivais, Lote 2.13.01A', 'Lisboa', 'Lisboa', '1990-231', 45.00, 15000, 1, 2, 1, 'aprovado'),

('Workshop de Fotografia Digital', 'Aprenda as técnicas mais modernas de fotografia digital com profissionais da área. Inclui sessão prática e certificado de participação.', '2025-08-05 09:00:00', '2025-08-05 17:00:00', 'Centro de Artes Visuais', 'Rua das Artes, 123', 'Porto', 'Porto', '4000-123', 25.00, 30, 6, 3, 2, 'aprovado'),

('Concerto de Fado Tradicional', 'Uma noite especial dedicada ao fado tradicional português com fadistas reconhecidos nacionalmente.', '2025-08-20 21:00:00', '2025-08-20 23:30:00', 'Casa do Fado', 'Rua do Fado, 45', 'Coimbra', 'Coimbra', '3000-456', 15.00, 100, 1, 1, 3, 'aprovado'),

('Conferência Tech 2025', 'A maior conferência de tecnologia do país. Palestras sobre IA, blockchain, desenvolvimento web e muito mais.', '2025-09-10 09:00:00', '2025-09-10 18:00:00', 'Centro de Congressos', 'Avenida da Tecnologia, 200', 'Braga', 'Braga', '4700-200', 0.00, 500, 4, 4, 1, 'aprovado'),

('Feira de Gastronomia Regional', 'Descubra os sabores tradicionais de Portugal numa feira gastronómica com produtos regionais e workshops de culinária.', '2025-08-28 10:00:00', '2025-08-30 22:00:00', 'Parque da Cidade', 'Parque da Cidade, s/n', 'Aveiro', 'Aveiro', '3800-300', 5.00, 2000, 5, 7, 2, 'aprovado'),

('Exposição de Arte Contemporânea', 'Exposição com obras de artistas contemporâneos portugueses e internacionais. Uma viagem pela arte moderna.', '2025-09-01 10:00:00', '2025-09-30 18:00:00', 'Museu de Arte Moderna', 'Praça das Artes, 1', 'Faro', 'Faro', '8000-100', 8.00, 200, 3, 5, 3, 'aprovado'),

('Espetáculo de Dança Moderna', 'Uma apresentação única de dança contemporânea pela companhia nacional de dança.', '2025-08-25 20:00:00', '2025-08-25 22:00:00', 'Teatro Municipal', 'Rua do Teatro, 10', 'Viseu', 'Viseu', '3500-150', 12.00, 300, 8, 8, 1, 'aprovado'),

('Competição de E-Sports', 'Torneio nacional de e-sports com os melhores jogadores do país. Prémios em dinheiro e transmissão ao vivo.', '2025-09-15 14:00:00', '2025-09-15 22:00:00', 'Arena Gaming', 'Centro Comercial TechCenter', 'Setúbal', 'Setúbal', '2900-400', 10.00, 150, 2, 6, 2, 'aprovado');

-- Inserir alguns interesses
INSERT INTO interesses (utilizador_id, evento_id) VALUES 
(2, 1), (3, 1), (2, 2), (3, 3), (2, 4), (3, 5), (2, 6), (3, 7);

-- Inserir algumas inscrições
INSERT INTO inscricoes (utilizador_id, evento_id, quantidade_bilhetes, valor_total, status, codigo_bilhete) VALUES 
(2, 1, 2, 90.00, 'confirmada', 'EU202500001'),
(3, 3, 1, 15.00, 'confirmada', 'EU202500002'),
(2, 4, 1, 0.00, 'confirmada', 'EU202500003'),
(3, 6, 1, 8.00, 'confirmada', 'EU202500004');
