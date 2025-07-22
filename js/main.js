// Funções utilitárias para o Events & U

document.addEventListener('DOMContentLoaded', function() {
    // Auto-hide alerts após 5 segundos
    const alerts = document.querySelectorAll('.alert');
    alerts.forEach(alert => {
        setTimeout(() => {
            alert.style.opacity = '0';
            setTimeout(() => {
                alert.remove();
            }, 300);
        }, 5000);
    });

    // Confirmação de eliminação
    const deleteButtons = document.querySelectorAll('.btn-delete');
    deleteButtons.forEach(button => {
        button.addEventListener('click', function(e) {
            if (!confirm('Tem a certeza que deseja eliminar este item?')) {
                e.preventDefault();
            }
        });
    });

    // Preview de imagem no upload
    const imageInputs = document.querySelectorAll('input[type="file"][accept*="image"]');
    imageInputs.forEach(input => {
        input.addEventListener('change', function(e) {
            const file = e.target.files[0];
            if (file) {
                const reader = new FileReader();
                reader.onload = function(e) {
                    let preview = document.getElementById('image-preview');
                    if (!preview) {
                        preview = document.createElement('img');
                        preview.id = 'image-preview';
                        preview.style.maxWidth = '200px';
                        preview.style.maxHeight = '200px';
                        preview.style.marginTop = '10px';
                        preview.style.borderRadius = '5px';
                        input.parentNode.appendChild(preview);
                    }
                    preview.src = e.target.result;
                };
                reader.readAsDataURL(file);
            }
        });
    });

    // Validação de formulários
    const forms = document.querySelectorAll('form[data-validate]');
    forms.forEach(form => {
        form.addEventListener('submit', function(e) {
            const requiredFields = form.querySelectorAll('[required]');
            let isValid = true;

            requiredFields.forEach(field => {
                if (!field.value.trim()) {
                    field.style.borderColor = '#dc3545';
                    isValid = false;
                } else {
                    field.style.borderColor = '#28a745';
                }
            });

            if (!isValid) {
                e.preventDefault();
                alert('Por favor, preencha todos os campos obrigatórios.');
            }
        });
    });

    // Pesquisa em tempo real (se existir)
    const searchInput = document.querySelector('#live-search');
    if (searchInput) {
        let searchTimeout;
        searchInput.addEventListener('input', function() {
            clearTimeout(searchTimeout);
            searchTimeout = setTimeout(() => {
                performLiveSearch(this.value);
            }, 300);
        });
    }
});

// Função para pesquisa em tempo real
function performLiveSearch(query) {
    if (query.length < 2) return;
    
    fetch(`pages/search-ajax.php?q=${encodeURIComponent(query)}`)
        .then(response => response.json())
        .then(data => {
            displaySearchResults(data);
        })
        .catch(error => {
            console.error('Erro na pesquisa:', error);
        });
}

// Exibir resultados da pesquisa
function displaySearchResults(results) {
    const resultsContainer = document.getElementById('search-results');
    if (!resultsContainer) return;

    if (results.length === 0) {
        resultsContainer.innerHTML = '<p>Nenhum resultado encontrado.</p>';
        return;
    }

    let html = '<div class="search-results-list">';
    results.forEach(result => {
        html += `
            <div class="search-result-item">
                <a href="pages/evento.php?id=${result.id}">
                    <h4>${result.titulo}</h4>
                    <p>${result.data_evento} - ${result.local}</p>
                </a>
            </div>
        `;
    });
    html += '</div>';
    
    resultsContainer.innerHTML = html;
}

// Função para mostrar loading
function showLoading() {
    const loader = document.createElement('div');
    loader.id = 'page-loader';
    loader.innerHTML = '<div class="spinner"></div>';
    loader.style.cssText = `
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background: rgba(255,255,255,0.8);
        display: flex;
        align-items: center;
        justify-content: center;
        z-index: 9999;
    `;
    document.body.appendChild(loader);
}

// Função para esconder loading
function hideLoading() {
    const loader = document.getElementById('page-loader');
    if (loader) {
        loader.remove();
    }
}

// Função para formatar datas
function formatDate(dateString) {
    const date = new Date(dateString);
    return date.toLocaleDateString('pt-PT', {
        year: 'numeric',
        month: 'long',
        day: 'numeric',
        hour: '2-digit',
        minute: '2-digit'
    });
}

// Função para validar email
function isValidEmail(email) {
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return emailRegex.test(email);
}

// Função para mostrar notificações
function showNotification(message, type = 'info') {
    const notification = document.createElement('div');
    notification.className = `alert alert-${type} notification`;
    notification.textContent = message;
    notification.style.cssText = `
        position: fixed;
        top: 20px;
        right: 20px;
        z-index: 10000;
        min-width: 300px;
        animation: slideIn 0.3s ease-out;
    `;
    
    document.body.appendChild(notification);
    
    setTimeout(() => {
        notification.style.animation = 'slideOut 0.3s ease-in';
        setTimeout(() => {
            notification.remove();
        }, 300);
    }, 4000);
}

// CSS para animações
const style = document.createElement('style');
style.textContent = `
    @keyframes slideIn {
        from { transform: translateX(100%); opacity: 0; }
        to { transform: translateX(0); opacity: 1; }
    }
    
    @keyframes slideOut {
        from { transform: translateX(0); opacity: 1; }
        to { transform: translateX(100%); opacity: 0; }
    }
    
    .spinner {
        width: 40px;
        height: 40px;
        border: 4px solid #444;
        border-top: 4px solid #f1c40f;
        border-radius: 50%;
        animation: spin 1s linear infinite;
    }
    
    @keyframes spin {
        0% { transform: rotate(0deg); }
        100% { transform: rotate(360deg); }
    }
`;
document.head.appendChild(style);
