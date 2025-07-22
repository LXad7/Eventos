@echo off
chcp 65001 >nul
echo ========================================
echo     ARENA FITNESS - SYNC RAPIDO
echo ========================================

cd /d "c:\xampp\htdocs\Eventos"

echo Puxando alteracoes do GitHub...
git pull origin Eventos

echo Adicionando arquivos alterados...
git add .

echo Verificando se ha alteracoes...
git diff --staged --quiet
if %errorlevel% neq 0 (
    echo Fazendo commit das alteracoes...
    git commit -m "Auto-update Arena Fitness: %date% %time%"
    
    echo Enviando para GitHub...
    git push origin Eventos
    
    echo ✅ Sincronizacao concluida!
    echo 🔗 GitHub: https://github.com/LXad7/Eventos
) else (
    echo ℹ️ Nenhuma alteracao encontrada.
)

echo.
pause
