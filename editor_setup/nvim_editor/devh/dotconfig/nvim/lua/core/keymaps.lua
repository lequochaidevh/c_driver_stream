vim.g.mapleader = " "

local keymap = vim.keymap.set
local opts = { noremap = true, silent = true }

-- Save / quit
keymap("n", "<leader>w", ":w<CR>", opts)
keymap("n", "<leader>q", ":q<CR>", opts)

-- Window
keymap("n", "<leader>sv", "<C-w>v", opts)
keymap("n", "<leader>sh", "<C-w>s", opts)

-- Terminal
keymap("n", "<leader>tt", ":terminal<CR>", opts)

-- Ctrl+S = save (normal + insert)
-- keymap({ "n", "i" }, "<C-s>", function()
--   vim.cmd("w")
-- end, { silent = true, desc = "Save file" })

-- keymap({ "n", "i" }, "<C-s>", "<cmd>w<CR>", {
--   silent = true,
--   desc = "Save file",
-- })