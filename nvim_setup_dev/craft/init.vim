" ======================
" Plugin manager
" ======================
call plug#begin('~/.local/share/nvim/plugged')

Plug 'nvim-tree/nvim-tree.lua'
Plug 'nvim-tree/nvim-web-devicons'
Plug 'nvim-lualine/lualine.nvim'
" Plug 'morhetz/gruvbox'
" Plug 'ellisonleao/gruvbox.nvim'
" Plug 'folke/tokyonight.nvim'
Plug 'EdenEast/nightfox.nvim'
" NOT USE Plug 'sphamba/smear-cursor.nvim'
Plug 'echasnovski/mini.animate'
" Ctrl + Shift + F
Plug 'nvim-lua/plenary.nvim'
Plug 'nvim-telescope/telescope.nvim', { 'tag': '0.1.4' }

call plug#end()
" colorscheme gruvbox
" colorscheme tokyonight

" nightfox
" colorscheme nordfox
colorscheme nightfox

" ======================
" Nvim-tree setup
" ======================
lua << EOF
require("nvim-tree").setup({
  filters = {
    dotfiles = false,   -- QUAN TRỌNG
  },
})

require('telescope').setup{}
EOF

" ======================
" Keymap
" ======================

set number
" set relativenumber
set mouse=a
set termguicolors
"highlight Normal guibg=none
"highlight NonText guibg=none
" ===== Transparent background (fix NvimTree re-color background) =====
"highlight Normal guibg=none
"highlight NormalNC guibg=none
"highlight NonText guibg=none
"highlight EndOfBuffer guibg=none
"
"" NvimTree
"highlight NvimTreeNormal guibg=none
"highlight NvimTreeNormalNC guibg=none
"highlight NvimTreeEndOfBuffer guibg=none
"highlight NvimTreeVertSplit guibg=none
"
"highlight NvimTreeVertSplit guifg=#444444 guibg=none

" ===== Sidebar sáng hơn code =====
highlight Normal guibg=none
highlight NormalNC guibg=none

" Code area (giữ nền trong suốt / tối)
highlight Normal guibg=none

" NvimTree sáng hơn
highlight NvimTreeNormal guibg=none
highlight NvimTreeNormalNC guibg=none
highlight NvimTreeEndOfBuffer guibg=none

" Border ngăn cách
highlight NvimTreeVertSplit guifg=#3c3c3c guibg=#1e1e2e
"file: ~/.config/nvim/init.vim

"lua << EOF
"require("smear_cursor").setup({
"  stiffness = 0.8,
"  trailing_stiffness = 0.5,
"  distance_stop_animating = 0.5,
"  hide_target_hack = true,
"})
"EOF

"lua << EOF
"local animate = require('mini.animate')
"
"local function smart_cursor_path()
"  local line_path = animate.gen_path.line()
"
"  return function(from, to)
"    -- ⚠️ mini.animate cũ có thể gọi path với to == nil
"    if not from or not to then
"      return nil
"    end
"
"    -- khác dòng → animate
"    if from[1] ~= to[1] then
"      return line_path(from, to)
"    end
"
"    -- cùng dòng, nhảy ngược xa → tắt
"    if to[2] < from[2] and (from[2] - to[2]) > 10 then
"      return nil
"    end
"
"    return line_path(from, to)
"  end
"end
"
"animate.setup({
"  cursor = {
"    enable = true,
"    timing = animate.gen_timing.linear({ duration = 110 }),
"    path   = smart_cursor_path(),
"  },
"
"  scroll = {
"    enable = true,
"    timing = animate.gen_timing.linear({ duration = 150 }),
"  },
"
"  resize = { enable = false },
"  open   = { enable = false },
"  close  = { enable = false },
"})
"EOF

lua << EOF
local telescope = require("telescope.builtin")

vim.keymap.set("n", "<leader>fs", function()
  telescope.live_grep({ cwd = vim.fn.expand("%:p:h") })
end)

vim.keymap.set("n", "<leader>fg", telescope.live_grep)
EOF

lua << EOF
require("nvim-tree").setup{}
vim.keymap.set("n", "<C-b>", ":NvimTreeToggle<CR>")
EOF
