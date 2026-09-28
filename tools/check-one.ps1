# ============================================================
#  单文件编译检查 —— 学习时最常用的命令
#
#  用法:  powershell -ExecutionPolicy Bypass -File tools\check-one.ps1 example\W2-Day4-array-char.c
#         powershell -ExecutionPolicy Bypass -File tools\check-one.ps1 w3-pointer-toolbox\main.c
#
#  提示:  直接跑 gcc 也能看到完整警告（推荐学会）：
#         gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only example\W2-Day2-prime-number.c
# ============================================================

param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$File
)

$Root   = Split-Path -Parent $PSScriptRoot
$target = if ([System.IO.Path]::IsPathRooted($File)) { $File } else { Join-Path $Root $File }

if (-not (Test-Path $target)) {
    Write-Host "找不到文件: $target"
    exit 1
}

$CFLAGS = @('-std=c11', '-Wall', '-Wextra', '-Wpedantic')
$rel    = $target.Substring($Root.Length + 1)

Write-Host ""
Write-Host "检查: $rel"
Write-Host ""

$tmp = [System.IO.Path]::GetTempFileName()
& gcc @CFLAGS -fdiagnostics-color=never -fsyntax-only $target 2> $tmp | Out-Null
$raw = @(Get-Content -LiteralPath $tmp -ErrorAction SilentlyContinue)
Remove-Item $tmp -Force -ErrorAction SilentlyContinue

if ($raw.Count -eq 0) {
    Write-Host "结论: 零警告零错误，通过"
    exit 0
}

# gcc 自身报错要显式暴露
$selfErr = @($raw | Where-Object { $_ -match 'unrecognized command line option|fatal error|internal compiler error' })
if ($selfErr.Count -gt 0) {
    Write-Host "!!! gcc 本身出错，结果不可信 !!!"
    $selfErr | ForEach-Object { Write-Host "  $_" }
    exit 1
}

# 只打印 位置 + 该行的关键点，避免长文本被终端截断
foreach ($l in $raw) {
    $short = $l -replace [regex]::Escape($Root + '\'), ''
    if ($short -match '^(.*?):(\d+):(\d+):\s*(warning|error):\s*(.*)$') {
        $file = $Matches[1]; $ln = $Matches[2]; $col = $Matches[3]; $kind = $Matches[4]; $msg = $Matches[5]
        # 提取警告类型标记（如果有）
        $opt = ''
        if ($msg -match '\[(-W[^\]]+)\]') { $opt = $Matches[1]; $msg = ($msg -replace '\s*\[-W[^\]]+\]', '') }
        # 消息压到 50 字符内，配合"具体位置"足够定位
        if ($msg.Length -gt 50) { $msg = $msg.Substring(0, 47) + '...' }
        Write-Host ("  {0}:{1}  {2}  {3}" -f $ln, $col, $opt, $msg)
    }
    elseif ($short -match 'warning:|error:') {
        Write-Host ("    $short")
    }
}

$warn = @($raw | Where-Object { $_ -match 'warning:' }).Count
$err  = @($raw | Where-Object { $_ -match 'error:' }).Count
Write-Host ""
Write-Host "结论: $warn 条警告, $err 条错误"
Write-Host ""
Write-Host "看完整警告文本，直接跑（不会被脚本截断）:"
Write-Host "  gcc $($CFLAGS -join ' ') -fsyntax-only $rel"
