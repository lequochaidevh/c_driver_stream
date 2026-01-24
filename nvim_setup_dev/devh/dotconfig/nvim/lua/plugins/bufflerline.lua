return {
  "akinsho/bufferline.nvim",
  version = "*",
  dependencies = "nvim-tree/nvim-web-devicons",
  config = function()
    require("bufferline").setup({
      options = {
        diagnostics = "nvim_lsp",
        separator_style = "slant",
        show_buffer_close_icons = true,
        show_close_icon = false,
      },
    })
  end,
}