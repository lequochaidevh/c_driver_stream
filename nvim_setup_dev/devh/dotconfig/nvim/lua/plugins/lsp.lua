return {
  "neovim/nvim-lspconfig",
  version = "v0.1.7", -- BẮT BUỘC cho nvim 0.9.x
  dependencies = {
    "hrsh7th/cmp-nvim-lsp",
  },
  config = function()
    local lspconfig = require("lspconfig")
    local capabilities = require("cmp_nvim_lsp").default_capabilities()

    lspconfig.clangd.setup({
      capabilities = capabilities,
    })
  end,
}

