# Digraph Tool

## About the Tool

This tool can be used to simplify the process of entering special characters.
Instead of remembering obscure alt-codes like Alt-0176 for a degree sign,
Alt-0233 for an e with an acute accent or Alt-230 for the Greek letter mu,
these can be entered by hitting a standard short-cut key (e.g. Win+K) followed
by two characters that are logically chosen to represent the symbol.  For the
examples above, the degree symbol uses the characters `DG`, the e with an
acute accent is `e'` and the letter mu is entered as `m*`.

The tool can also be useful on laptops that don't have a numeric keypad, upon
which the Alt key codes rely.

There are some examples of common digraphs in `docs/digraph_table.xlsx` (the
first section is standard suffixes such as `'` for an acute accent or `*` for
Greek letters, the others show specific digraphs).  There's also a PDF of this
if you don't have anything that will read xlsx spreadsheets or just want a
print-out.

The digraphs were taken from those used in [Vim text editor][vim]; a full list
is available at [http://vimhelp.appspot.com/digraph.txt.html#digraph-table][vimlist].

The special digraph "VE" will show the version number of the application.

Note that the tool works by inserting the symbol into the clipboard and then
sending `Ctrl-V` to the last active window.  This has a few implications:

1. It will overwrite whatever was previously in the clipboard.
2. If Windows is going a bit slowly, the paste will fail as the window switch
   won't have happened in time.  In this case, just press `Ctrl-V` manually.
3. Applications that don't use `Ctrl-V` as paste (for example [Vim][vim] with
   the default bindings) won't paste the character in so you'll have to do it
   manually (on in the case of [Vim][vim], just use the [built-in digraph
   support][vimdig]).

You can override the pasting commands using `config/windows.ini`.  With the
default configuration, `Ctrl-R, +` is sent if the last active window included
the text "GVIM" in the title.  Otherwise `Ctrl-V` is sent.

## Keyboard Shortcut Prefix

To create a keyboard shortcut, there are two methods that work equally well:

* Create a Windows shortcut on your start menu and set the shortcut key appropriately.
* Use [AutoHotKey (https://autohotkey.com/)][autohotkey] and install a simple
  script to run the executable when you press the chosen key.  An example autohotkey
  script is included in the `docs` folder.

The second option is probably a bit faster, but either should work.

## Other Operating Systems

The tool is written using the Qt graphical user interface library.  In principle, it
should be straightforward to port it to Linux or Mac OS X.  The only Windows-specific
parts of the code are in `windows_specific.cpp` and are responsible for sending the
"paste" keyboard shortcut to the last active application.

## Known Issues

1. If certain applications are the foreground Window when the application starts, the
   clipboard is not populated.  I have absolutely no idea why not.  Windows Notepad is one
   example, Vivaldi Web browser is another.  Microsoft Office apps work okay.

[autohotkey]: https://autohotkey.com/
[vimlist]: http://vimhelp.appspot.com/digraph.txt.html#digraph-table
[vimdig]: http://vimhelp.appspot.com/digraph.txt.html#digraphs-use
[vim]: http://www.vim.org


<!-- vim: set ft=markdown tw=90 : -->
