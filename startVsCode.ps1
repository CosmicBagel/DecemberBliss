Write-Host "Starting vs code with dev environment"
Set-Location -Path "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\"
& { Import-Module "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll";
    Enter-VsDevShell 259f100b }
Set-Location -Path "C:\Users\samue\OneDrive\Dev\DecemberBliss"
code .