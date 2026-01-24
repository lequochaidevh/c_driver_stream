
-- Plug 'nvim-tree/nvim-tree.lua'
-- Plug 'nvim-tree/nvim-web-devicons'

return {
  {
    "nvim-neo-tree/neo-tree.nvim",
    branch = "v3.x",
    dependencies = {
      "nvim-lua/plenary.nvim",
      "MunifTanjim/nui.nvim",
      "nvim-tree/nvim-web-devicons", -- optional, but recommended
    },
    lazy = false, -- neo-tree will lazily load itself
    config = function() 
      vim.keymap.set("n", "<leader>v", "<cmd>Neotree filesystem reveal left<CR>", { desc = "NeoTree reveal" })
      vim.keymap.set("n", "<C-b>", "<cmd>Neotree toggle<CR>", { desc = "Toggle NvimTree" })
    end,
  }
}
