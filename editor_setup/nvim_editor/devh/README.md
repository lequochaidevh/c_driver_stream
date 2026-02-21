

```text
# tree
~/.config/nvim/
├── init.lua
├── lua/
│   ├── core/
│   │   ├── options.lua      -- set option
│   │   ├── keymaps.lua      -- keymap chung
│   │   ├── autocmds.lua     -- autocmd
│   │   └── globals.lua      -- biến global
│   ├── plugins/
│   │   ├── init.lua         -- khai báo plugin
│   │   ├── lsp.lua          -- LSP + C/C++
│   │   ├── cmp.lua          -- autocomplete
│   │   ├── treesitter.lua
│   │   ├── telescope.lua
│   │   ├── nvimtree.lua
│   │   └── ui.lua           -- theme, statusline, animation
│   └── utils/
│       └── helpers.lua
```

**Create foulder**

```sh
./create_nvim_tree.sh
```
