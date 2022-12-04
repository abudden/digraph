#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

; Adjust this to suit the place you've installed digraph.exe
DigraphPath := "c:\path\to\digraph.exe"

return

; Re-map Win+J and Win+K to run digraph.exe
#k::
#j::
SetTitleMatchMode  2 ; window contains
if FileExist(DigraphPath)
{
	Run, % DigraphPath
	; Wait until the window pops up, then bring it to the front
	WinWait, Digraph
	WinActivate, Digraph
}
