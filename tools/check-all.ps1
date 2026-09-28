# ============================================================
#  编译体检脚本 —— 检查 c-learning 下所有 .c 文件的警告
#
#  用法:  powershell -ExecutionPolicy Bypass -File tools\check-all.ps1
#
#  说明:  完整诊断写入 tools\warnings.log（终端可能显示不全长行）。
#         要逐条看，用文本编辑器打开该文件，或直接跑 gcc：
#           gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only <文件>
#
#  实现要点: 必须用 cmd 的 2>> 重定向收集 gcc 输出。
#            用 PowerShell 的 2> 会把每条诊断截断到 76 字符。
# ============================================================

$ErrorActionPreference = 'Continue'

$Root    = Split-Path -Parent $PSScriptRoot
$LogFile = Join-Path $PSScriptRoot 'warnings.log'
$CFLAGS  = '-std=c11 -Wall -Wextra -Wpedantic -fdiagnostics-color=never'

Write-Host ""
Write-Host "===== c-learning 编译体检 ====="
Write-Host "根目录: $Root"
Write-Host "选项  : $CFLAGS"
Write-Host ""

$files = @(Get-ChildItem -LiteralPath $Root -Recurse -File -Filter *.c |
           Where-Object { $_.FullName -notmatch '\\(output|build|\.git)\\' } |
           Sort-Object FullName)

if ($files.Count -eq 0) { Write-Host "没找到 .c 文件"; exit 1 }

# 清空日志
Set-Content -LiteralPath $LogFile -Value '' -NoNewline -Encoding UTF8

# 逐文件用 cmd 重定向收集完整诊断
$status = @()
foreach ($f in $files) {
    $tmp = [System.IO.Path]::GetTempFileName()
    cmd /c "gcc $CFLAGS -fsyntax-only `"$($f.FullName)`" 2>`"$tmp`""
    $raw = @(Get-Content -LiteralPath $tmp -ErrorAction SilentlyContinue)
    Remove-Item $tmp -Force -ErrorAction SilentlyContinue

    $rel  = $f.FullName.Substring($Root.Length + 1)
    $warn = @($raw | Where-Object { $_ -match 'warning:' }).Count
    $err  = @($raw | Where-Object { $_ -match 'error:' }).Count

    # gcc 自身出错（选项不支持等）必须显式暴露，绝不能被当成"零警告"
    $selfErr = @($raw | Where-Object { $_ -match 'unrecognized command line option|fatal error|internal compiler error|cannot execute' })
    foreach ($l in $selfErr) { Add-Content -LiteralPath $LogFile -Value "[GCC ERROR] $rel :: $l" -Encoding UTF8 }

    $status += [pscustomobject]@{ File = $rel; Warn = $warn; Err = $err; SelfErr = $selfErr.Count }

    if ($warn -gt 0 -or $err -gt 0) {
        Add-Content -LiteralPath $LogFile -Value '' -Encoding UTF8
        Add-Content -LiteralPath $LogFile -Value "========== $rel ==========" -Encoding UTF8
        foreach ($l in $raw) {
            Add-Content -LiteralPath $LogFile -Value ($l -replace [regex]::Escape($Root + '\'), '') -Encoding UTF8
        }
    }
}

$clean   = @($status | Where-Object { $_.Warn -eq 0 -and $_.Err -eq 0 }).Count
$bad     = @($status | Where-Object { $_.Warn -gt 0 -or $_.Err -gt 0 })
$totalW  = ($status | Measure-Object Warn -Sum).Sum
$totalE  = ($status | Measure-Object Err -Sum).Sum
$selfErrTotal = ($status | Measure-Object SelfErr -Sum).Sum

Write-Host "---------------------------------------------"
Write-Host ("文件总数 : {0}" -f $status.Count)
Write-Host ("零警告   : {0}" -f $clean)
Write-Host ("有问题的 : {0}" -f $bad.Count)
Write-Host ("警告条数 : {0}" -f $totalW)
Write-Host ("错误条数 : {0}" -f $totalE)
Write-Host "---------------------------------------------"

if ($selfErrTotal -gt 0) {
    Write-Host ""
    Write-Host "!!! gcc 本身报错了，上面的统计不可信，先修工具链 !!!"
}

if ($totalW -eq 0 -and $totalE -eq 0) {
    Write-Host ""
    Write-Host "全部文件零警告零错误，本周目标达成！"
    exit 0
}

Write-Host ""
Write-Host "===== 需要修的文件（按警告数排序） ====="
$bad | Sort-Object Warn -Descending | ForEach-Object {
    Write-Host ("  {0,2} 条  {1}" -f $_.Warn, $_.File)
}

Write-Host ""
Write-Host "完整诊断: tools\warnings.log"
Write-Host "逐条查看某个文件: gcc $CFLAGS -fsyntax-only <文件路径>"
Write-Host ""
