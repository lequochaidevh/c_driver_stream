return {
  {
    "nvim-neo-tree/neo-tree.nvim",
    branch = "v3.x",
    dependencies = {
      "nvim-lua/plenary.nvim",
      "MunifTanjim/nui.nvim",
      "nvim-tree/nvim-web-devicons",
    },
    lazy = false,

    config = function()
      require("neo-tree").setup({
        filesystem = {
          filtered_items = {
            visible = true,          -- QUAN TRỌNG
            hide_dotfiles = false,   -- hiện file .xxx
            hide_gitignored = false, -- hiện file gitignore
          },
        },
      })

      -- Keymaps
      vim.keymap.set(
        "n",
        "<leader>v",
        "<cmd>Neotree filesystem reveal left<CR>",
        { desc = "NeoTree reveal" }
      )

      vim.keymap.set(
        "n",
        "<C-b>",
        "<cmd>Neotree toggle<CR>",
        { desc = "Toggle NeoTree" }
      )
    end,
  },
}

