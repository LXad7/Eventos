@echo off
chcp 65001 >nul
title Arena Fitness - Auto Sync
color 0A

echo.
echo =====================================
echo        ARENA FITNESS AUTO SYNC
echo =====================================
echo.

cd /d "c:\xampp\htdocs\Eventos"

echo [1] Verificando status do repositorio...
git status --porcelain > nul 2>&1
if %errorlevel% neq 0 (
    echo ❌ Erro: Nao e um repositorio Git valido
    pause
    exit /b 1
)

echo [2] Puxando ultimas alteracoes do GitHub...
git pull origin Eventos

echo [3] Adicionando arquivos modificados...
git add .

echo [4] Verificando alteracoes...
git diff --staged --quiet
if %errorlevel% neq 0 (
    echo [5] Alteracoes encontradas - fazendo commit...
    
    set /p commit_msg="Digite uma mensagem de commit (ou Enter para automatica): "
    if "%commit_msg%"=="" (
        set commit_msg=Auto-update Arena Fitness - %date% %time%
    )
    
    git commit -m "%commit_msg%"
    
    if %errorlevel% equ 0 (
        echo [6] Enviando para GitHub...
        git push origin Eventos
        
        if %errorlevel% equ 0 (
            echo.
            echo ✅ Sincronizacao concluida com sucesso!
            echo 🔗 Repositorio: https://github.com/LXad7/Eventos
        ) else (
            echo ❌ Erro ao enviar para GitHub
        )
    ) else (
        echo ❌ Erro no commit
    )
) else (
    echo ℹ️ Nenhuma alteracao encontrada - repositorio ja esta atualizado
)

echo.
echo Pressione qualquer tecla para continuar...
pause > nul
