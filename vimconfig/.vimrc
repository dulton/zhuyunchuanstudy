" All system-wide defaults are set in $VIMRUNTIME/debian.vim (usually just
" /usr/share/vim/vimcurrent/debian.vim) and sourced by the call to :runtime
" you can find below.  If you wish to change any of those settings, you should
" do it in this file (/etc/vim/vimrc), since debian.vim will be overwritten
" everytime an upgrade of the vim packages is performed.  It is recommended to
" make changes after sourcing debian.vim since it alters the value of the
" 'compatible' option.

" This line should not be removed as it ensures that various options are
" properly set to work with the Vim-related packages available in Debian.
runtime! debian.vim

" Uncomment the next line to make Vim more Vi-compatible
" NOTE: debian.vim sets 'nocompatible'.  Setting 'compatible' changes numerous
" options, so any other options should be set AFTER setting 'compatible'.
"set compatible

" Vim5 and later versions support syntax highlighting. Uncommenting the
" following enables syntax highlighting by default.
if has("syntax")
  syntax on
endif

" If using a dark background within the editing area and syntax highlighting
" turn on this option as well
"set background=dark

" Uncomment the following to have Vim jump to the last position when
" reopening a file
"if has("autocmd")
"  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
"endif

" Uncomment the following to have Vim load indentation rules and plugins
" according to the detected filetype.
"if has("autocmd")
"  filetype plugin indent on
"endif

" The following are commented out as they cause vim to behave a lot
" differently from regular Vi. They are highly recommended though.
set showcmd		" Show (partial) command in status line.
"set showmatch		" Show matching brackets.
"set ignorecase		" Do case insensitive matching
set smartcase		" Do smart case matching
set incsearch		" Incremental search
"set autowrite		" Automatically save before commands like :next and :make
"set hidden             " Hide buffers when they are abandoned
set mouse=a		" Enable mouse usage (all modes)

" Source a global configuration file if available
if filereadable("/etc/vim/vimrc.local")
  source /etc/vim/vimrc.local
endif

if(has("win32") || has("win95") || has("win64") || has("win16")) "ÅÐ¶šµ±Ç°²Ù×÷ÏµÍ³ÀàÐÍ
    let g:iswindows=1
else
    let g:iswindows=0
endif
set nocompatible "²»ÒªvimÄ£·ÂviÄ£Êœ£¬œšÒéÉèÖÃ£¬·ñÔò»áÓÐºÜ¶à²»ŒæÈÝµÄÎÊÌâ
syntax on
if has("autocmd")
    filetype plugin indent on "žùŸÝÎÄŒþœøÐÐËõœø
    augroup vimrcEx
        au!
        autocmd FileType text setlocal textwidth=78
        autocmd BufReadPost *
                    \ if line("¡®\"") > 1 && line(" '\" ") <= line("$") | "ÊµÏÖŽò¿ªÍ¬Ò»ÎÄŒþÊ±£¬vimÄÜ¹»×Ô¶¯ŒÇ×¡ÉÏÒ»ŽÎµÄÎ»ÖÃ
                    \ exe "normal! g`\"" |
                    \ endif
    augroup END
else
    set autoindent " always set autoindenting on "ÖÇÄÜËõœø£¬ÏàÓŠµÄÓÐcindent£¬¹Ù·œËµautoindent¿ÉÒÔÖ§³Öž÷ÖÖÎÄŒþµÄËõœø£¬µ«ÊÇÐ§¹û»á±ÈÖ»Ö§³ÖC/C++µÄcindentÐ§¹û»á²îÒ»µã£¬µ«±ÊÕß²¢Ã»ÓÐ¿Ž³öÀŽ
endif " has("autocmd¡±)
set tabstop=4 "ÈÃÒ»žötabµÈÓÚ4žö¿Õžñ
set nowrap "²»×Ô¶¯»»ÐÐ
set hlsearch "žßÁÁÏÔÊŸœá¹û
set incsearch "ÔÚÊäÈëÒªËÑË÷µÄÎÄ×ÖÊ±£¬vim»áÊµÊ±Æ¥Åä
set backspace=indent,eol,start whichwrap+=<,>,[,] "ÔÊÐíÍËžñŒüµÄÊ¹ÓÃ
if(g:iswindows==1) "ÔÊÐíÊó±êµÄÊ¹ÓÃ
    "·ÀÖ¹linuxÖÕ¶ËÏÂÎÞ·š¿œ±Ž
    if has('mouse')
        set mouse=a
    endif
    au GUIEnter * simalt ~x
endif
"×ÖÌåµÄÉèÖÃ
set guifont=Monospace

set nu

"color
colorscheme desert


"ctag and cscope
map <F12> :call Do_CsTag()<CR>
"nmap <C-@>s :cs find s <C-R>=expand("<cword>")<CR><CR>:copen<CR>
"nmap <C-@>g :cs find g <C-R>=expand("<cword>")<CR><CR>
"nmap <C-@>c :cs find c <C-R>=expand("<cword>")<CR><CR>:copen<CR>
"nmap <C-@>t :cs find t <C-R>=expand("<cword>")<CR><CR>:copen<CR>
"nmap <C-@>e :cs find e <C-R>=expand("<cword>")<CR><CR>:copen<CR>
"nmap <C-@>f :cs find f <C-R>=expand("<cfile>")<CR><CR>:copen<CR>
"nmap <C-@>i :cs find i ^<C-R>=expand("<cfile>")<CR>$<CR>:copen<CR>
"nmap <C-@>d :cs find d <C-R>=expand("<cword>")<CR><CR>:copen<CR>
function Do_CsTag()
    let dir = getcwd()
    if filereadable("tags")
        if(g:iswindows==1)
            let tagsdeleted=delete(dir."\\"."tags")
        else
            let tagsdeleted=delete("./"."tags")
        endif
        if(tagsdeleted!=0)
            echohl WarningMsg | echo "Fail to do tags! I cannot delete the tags" | echohl None
            return
        endif
    endif
    if has("cscope")
        silent! execute "cs kill -1"
    endif
    if filereadable("cscope.files")
        if(g:iswindows==1)
            let csfilesdeleted=delete(dir."\\"."cscope.files")
        else
            let csfilesdeleted=delete("./"."cscope.files")
        endif
        if(csfilesdeleted!=0)
            echohl WarningMsg | echo "Fail to do cscope! I cannot delete the cscope.files" | echohl None
            return
        endif
    endif
    if filereadable("cscope.out")
        if(g:iswindows==1)
            let csoutdeleted=delete(dir."\\"."cscope.out")
        else
            let csoutdeleted=delete("./"."cscope.out")
        endif
        if(csoutdeleted!=0)
            echohl WarningMsg | echo "Fail to do cscope! I cannot delete the cscope.out" | echohl None
            return
        endif
    endif
    if(executable('ctags'))
        "silent! execute "!ctags -R šCc-types=+p šCfields=+S *"
        silent! execute "!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q ."
    endif
    if(executable('cscope') && has("cscope") )
        if(g:iswindows!=1)
            silent! execute "!find . -name ¡®*.h¡¯ -o -name ¡®*.c¡¯ -o -name ¡®*.cpp¡¯ -o -name ¡®*.java¡¯ -o -name ¡®*.cs¡¯ > cscope.files"
        else
            silent! execute "!dir /s/b *.c,*.cpp,*.h,*.java,*.cs >> cscope.files"
        endif
        silent! execute "!cscope -b"
        execute "normal :"
        if filereadable("cscope.out")
            execute "cs add cscope.out"
        endif
    endif
endfunction

"œøÐÐTlistµÄÉèÖÃ
"TlistUpdate¿ÉÒÔžüÐÂtags
map <F3> :silent! Tlist<CR> 
"°ŽÏÂF3ŸÍ¿ÉÒÔºô³öÁË
let Tlist_Ctags_Cmd= 'ctags' "ÒòÎªÎÒÃÇ·ÅÔÚ»·Ÿ³±äÁ¿Àï£¬ËùÒÔ¿ÉÒÔÖ±œÓÖŽÐÐ
let Tlist_Use_Right_Window=1 "ÈÃŽ°¿ÚÏÔÊŸÔÚÓÒ±ß£¬0µÄ»°ŸÍÊÇÏÔÊŸÔÚ×ó±ß
let Tlist_Show_One_File=0 "ÈÃtaglist¿ÉÒÔÍ¬Ê±Õ¹ÊŸ¶àžöÎÄŒþµÄº¯ÊýÁÐ±í£¬Èç¹ûÏëÖ»ÓÐ1žö£¬ÉèÖÃÎª1
let Tlist_File_Fold_Auto_Close=1 "·Çµ±Ç°ÎÄŒþ£¬º¯ÊýÁÐ±íÕÛµþÒþ²Ø
let Tlist_Exit_OnlyWindow=1 "µ±taglistÊÇ×îºóÒ»žö·ÖžîŽ°¿ÚÊ±£¬×Ô¶¯ÍÆ³övim
"ÊÇ·ñÒ»Ö±ŽŠÀítags.1:ŽŠÀí;0:²»ŽŠÀí
let Tlist_Process_File_Always=0 "²»ÊÇÒ»Ö±ÊµÊ±žüÐÂtags£¬ÒòÎªÃ»ÓÐ±ØÒª
let Tlist_Inc_Winwidth=0
"ÉèÖÃomnicppcomplete
set nocp
filetype plugin on
set completeopt=menu
"加速自动补全
let g:SuperTabRetainCompletionType=2
let g:SuperTabDefaultCompletionType="<C-X><C-O>"
"winmanager
let g:winManagerWindowLayout='FileExplorer|TagList'
nmap wm :WMToggle<cr>
"自动注释设置
map fg : Dox<cr>
let g:DoxygenToolkit_authorName="dantezhu"
let g:DoxygenToolkit_licenseTag="My own license\<enter>"
let g:DoxygenToolkit_undocTag="DOXIGEN_SKIP_BLOCK"
let g:DoxygenToolkit_briefTag_pre = "@brief\t"
let g:DoxygenToolkit_paramTag_pre = "@param\t"
let g:DoxygenToolkit_returnTag = "@return\t"
let g:DoxygenToolkit_briefTag_funcName = "no"
let g:DoxygenToolkit_maxFunctionProtoLines = 30
"minibufconfig
let g:miniBufExplMapCTabSwitchBufs = 1
let g:miniBufExplMapWindowNavVim = 1	"can use <C-h,j,k.l> to move betwen buffer 
