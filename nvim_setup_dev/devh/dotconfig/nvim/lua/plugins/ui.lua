return {
   {
    "catppuccin/nvim",
    name = "catppuccin",
    priority = 1000, -- rất quan trọng, load sớm
    config = function()
      require("catppuccin").setup({
        flavour = "mocha",
        transparent_background = true,
        show_end_of_buffer = false,
        term_colors = true,
        integrations = {
          nvimtree = true,
          treesitter = true,
          telescope = true,
        },
      })

      vim.cmd.colorscheme("catppuccin-mocha")

      -- ép highlight SAU colorscheme
      local hl = vim.api.nvim_set_hl
      hl(0, "Normal", { bg = "none" })
      hl(0, "NormalFloat", { bg = "none" })
      hl(0, "SignColumn", { bg = "none" })
      hl(0, "EndOfBuffer", { bg = "none" })
    end,
  },  
}