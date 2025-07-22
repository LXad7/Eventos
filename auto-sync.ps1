# Script PowerShell para sincronização automática
# Execute como administrador se quiser criar tarefa agendada

$projectPath = "C:\xampp\htdocs\Eventos"
$logFile = "$projectPath\sync.log"

function Write-Log {
    param($Message)
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    "$timestamp - $Message" | Add-Content $logFile
    Write-Host $Message
}

try {
    Set-Location $projectPath
    Write-Log "Iniciando sincronização automática..."
    
    # Pull das alterações remotas
    git pull origin Eventos 2>&1 | Add-Content $logFile
    
    # Adicionar arquivos
    git add . 2>&1 | Add-Content $logFile
    
    # Verificar se há alterações
    $changes = git diff --staged --name-only
    if ($changes) {
        Write-Log "Alterações encontradas: $($changes.Count) arquivos"
        
        # Commit automático
        $commitMsg = "Auto-sync Arena Fitness - $(Get-Date -Format 'dd/MM/yyyy HH:mm')"
        git commit -m $commitMsg 2>&1 | Add-Content $logFile
        
        # Push para GitHub
        git push origin Eventos 2>&1 | Add-Content $logFile
        Write-Log "✅ Sincronização concluída com sucesso!"
    } else {
        Write-Log "ℹ️ Nenhuma alteração para sincronizar"
    }
} catch {
    Write-Log "❌ Erro na sincronização: $($_.Exception.Message)"
}
