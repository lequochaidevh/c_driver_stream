#!/usr/bin/bash

set -e

NVIM_DIR="./dotconfig/nvim"

echo " Creating Neovim config structure at: $NVIM_DIR"

# Root
mkdir -p "$NVIM_DIR/lua/core"
mkdir -p "$NVIM_DIR/lua/plugins"
mkdir -p "$NVIM_DIR/lua/utils"

# Core files
touch "$NVIM_DIR/init.lua"
touch "$NVIM_DIR/lua/core/options.lua"
touch "$NVIM_DIR/lua/core/keymaps.lua"
touch "$NVIM_DIR/lua/core/autocmds.lua"
touch "$NVIM_DIR/lua/core/globals.lua"

# Plugin files
touch "$NVIM_DIR/lua/plugins/init.lua"
touch "$NVIM_DIR/lua/plugins/lsp.lua"
touch "$NVIM_DIR/lua/plugins/cmp.lua"
touch "$NVIM_DIR/lua/plugins/treesitter.lua"
touch "$NVIM_DIR/lua/plugins/telescope.lua"
touch "$NVIM_DIR/lua/plugins/nvimtree.lua"
touch "$NVIM_DIR/lua/plugins/ui.lua"
touch "$NVIM_DIR/lua/plugins/catppuccin.lua"

# Utils
touch "$NVIM_DIR/lua/utils/helpers.lua"

echo "Done! Structure created:"
tree "$NVIM_DIR" 2>/dev/null || find "$NVIM_DIR" -type f

